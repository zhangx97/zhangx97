#include "Script/lightEngine/light_engine.h"
#include <QThread>
#include <QDebug>
#include <QDir>
#include <QDateTime>
#include <math.h>

#define printf qDebug

extern "C"
{
    //#include "lible.c"
}

LightEngine::LightEngine(int lightType):lightSensorType(lightType)
{
    cyusb = new QLibrary("libcyusbserial");
    if(cyusb->load())
    {
        qDebug()<<"开始在库文件中检索";
        APIInit = (CyAPIInit)cyusb->resolve("cyAPIInit");
        SetProjectorOnOff = (CySetProjectorOnOff)cyusb->resolve("SetProjectorOnOff");
        SetLedOnOff = (CySetLedOnOff)cyusb->resolve("SetLedOnOff");
        GetLedOnOff = (CyGetLedOnOff)cyusb->resolve("GetLedOnOff");
        SetLedCurrent = (CySetLedCurrent)cyusb->resolve("SetLedCurrent");
        GetLedCurrent = (CyGetLedCurrent)cyusb->resolve("GetLedCurrent");
        FlashBlockRead = (CyFlashBlockRead)cyusb->resolve("FlashBlockRead");
        GetTemperature = (CyGetTemperature)cyusb->resolve("GetTemperature");
        GetLight = (CyGetLight)cyusb->resolve("GetLight");
        SetTestPattern = (CySetTestPattern)cyusb->resolve("SetTestPattern");
        FlashBlockReadMask = (CyFlashBlockReadMask)cyusb->resolve("FlashBlockReadMask");
        CyLibraryExit = (CyCyLibraryExit)cyusb->resolve("CyLibraryExit");
        freegl = (Cyfreegl)cyusb->resolve("freegl");
        //device0_num = (CyDEVICE0_NUM)cyusb.resolve("DEVICE0_NUM");
    }else
    {
        qDebug()<<"库文件加载失败";
        qDebug()<<cyusb->errorString();
    }
}

int LightEngine::InitLED()
{
    qDebug()<<"进入光机初始化函数";
    if(ProjOn()<0)
    {
        qDebug()<<"执行 ProjOn 失败";
        return -1;
    }
    qDebug()<<"等待一秒";
    QThread::sleep(1);
    char array_type[20];
    FlashBlockReadMask(maskdir,array_type);
    lightengineSN = array_type;
    if(ProjOff()<0)
    {
        return -1;
    }
    qDebug()<<"LED初始化完成";
    return 0;
}

int LightEngine::ReadLEDState()
{
    int rd_led = 1;
    int * rd_led_en_r = 0;
    int * rd_led_en_g = &rd_led;
    int * rd_led_en_b = 0;
    int status = GetLedOnOff(*device0_num,I2C_IF_NUM,rd_led_en_r,rd_led_en_g,rd_led_en_b);
    if(status != 0)
    {
        qWarning()<<"读取LED状态失败";
        return -1;
    }
    if(lightSensorType == 0)
    {
        return *rd_led_en_g;
    }else
    {
        return *rd_led_en_b;
    }
}

int LightEngine::LEDOn()
{
    int led_en_r = 1 ;
    int led_en_g = 1 ;
    int led_en_b = 1 ;
    int status = SetLedOnOff(*device0_num,I2C_IF_NUM,led_en_r,led_en_g,led_en_b);
    if (status != 0)
    {
        qWarning()<<"LED开启失败";
        return -1;
    }
    return status;
}

int LightEngine::LEDOff()
{
    int led_en_r = 0 ;
    int led_en_g = 0 ;
    int led_en_b = 0 ;
    int status = SetLedOnOff(*device0_num,I2C_IF_NUM,led_en_r,led_en_g,led_en_b);
    if (status != 0)
    {
        qWarning()<<"LED关闭失败";
        return -1;
    }
    return status;
}

int LightEngine::SetLightValue(int targetLight,int offset)
{
    int currentValue = ReadLEDCurrentValue();
    int sensorValue = ReadLEDSensorValue();
    qDebug()<<QString("当前电流：%1；当前亮度：%2；目标亮度：%3；").arg(currentValue).arg(sensorValue).arg(targetLight);
    if (currentValue == -1 && sensorValue == -1)
    {
        return -1;
    }
    if(abs(sensorValue - targetLight)>offset)
    {
        int targetCurrent = (currentValue/sensorValue)*targetLight;
        if (targetCurrent>600)
        {
            if (firstWithoutAdjust == true)
            {
                WriteLEDCurrentValue(targetCurrent);
                return 2;
            }
        }
        return 0;
    }else
    {
        return 1;
    }
}

int LightEngine::AutoSetLightValue(int targetLight,int offset)
{
    QThread::sleep(1);
    if(firstWithoutAdjust)
    {
        LEDOn();
        QThread::sleep(1);
        WriteLEDCurrentValue(100);
    }
    int setCount = 0;
    while(setCount <18)
    {
        if(!readyState)
        {
            return 0;
        }
        int SetLightValueReturn = SetLightValue(targetLight,offset);
        if(SetLightValueReturn == 2 && setCount == 0)
        {
            targetLight += 200;
            WriteLEDCurrentValue(100);
        }
        if(SetLightValueReturn == 1)
        {
            return 0;
        }else if(SetLightValueReturn < 0)
        {
            break;
        }else
        {
            setCount += 1;
        }
        QThread::sleep(1);
    }
    return -1;
}

//感觉没有必要
int LightEngine::LightValueFinalCalibrate(int targetLight,int offset)
{
    qDebug()<<"开始进行最终校准";
    int deltaValueCanAccept = 5;
    int downedSensorValue = ReadLEDSensorValue();
    qDebug()<<"获取当前时间戳";
    QDateTime toStartTime = QDateTime::currentDateTime();
    qDebug()<<QString("开始时间：%1").arg(toStartTime.toString());
    unsigned int startTime = toStartTime.toTime_t();
    unsigned int timeMaxSpan = 300;
    unsigned int timeCross = 0;
    bool lightValueStableFlag = false;
    qDebug()<<"尝试靠近标准值";
    while(timeCross<timeMaxSpan)
    {
        if(!readyState)
        {
            return 0;
        }
        QThread::sleep(10);
    }
}

int LightEngine::WriteLEDCurrentValue(int currentValue)
{
    if(currentValue<1 || currentValue>1023)
    {
        return -1;
    }
    int current_r = currentValue;
    int current_g = currentValue;
    int current_b = currentValue;
    int status = SetLedCurrent(*device0_num,I2C_IF_NUM,current_r,current_g,current_b);
    if(status!=0)
    {
        qWarning()<<"写入电流失败";
        return -1;
    }
    return status;
}

int LightEngine::ReadLEDCurrentValue()
{
    int rd_current = 100;
    int * rd_current_r = &rd_current;
    int * rd_current_g = 0;
    int * rd_current_b = &rd_current;
    int status = GetLedCurrent(*device0_num, I2C_IF_NUM, rd_current_r, rd_current_g, rd_current_b);
    if(status != 0)
    {
        qWarning()<<"读取电流失败";
        return -1;
    }
    if(lightSensorType == 0)
    {
        return *rd_current_g;
    }else
    {
        return *rd_current_b;
    }
}

int LightEngine::ReadLEDSensorValue()
{
    int * lightValue = 0;
    int status = GetLight(*device0_num, I2C_IF_NUM, lightValue);
    if(status !=0)
    {
        qWarning()<<"读取亮度失败";
        return -1;
    }
    int readLightValue = *lightValue;
    return readLightValue;
}

int LightEngine::ReadLEDTemp()
{
    int * tempValue = 0;
    int status = GetTemperature(*device0_num, I2C_IF_NUM, tempValue);
    if(status!=0)
    {
        qWarning()<<"读取温度失败";
        return -1;
    }
    if(lightSensorType == 0)
    {
        int tempValueToZero = *tempValue/10;
        return tempValueToZero;
    }else
    {
        int tempValueToOne = *tempValue;
        return tempValueToOne;
    }
}

int LightEngine::ProjOn()
{
    qDebug()<<"进入 proj on 函数";
    int status = APIInit(lightSensorType);
    if (status!=0)
    {
        qWarning()<<"cypress API 初始化失败";
        qWarning()<<lightSensorType;
        qWarning()<<status;
        return -1;
    }
    qDebug()<<"开始读取设备号";
    if(cyusb->load())
    {
    device0_num = (int *)cyusb->resolve("DEVICE0_NUM");                                         //得想其他办法取到这个值
    }
    status = SetProjectorOnOff(*device0_num, GPIO_IF_NUM, projector_en);
    if(status!=0)
    {
        qWarning()<<"光机初始化失败";
        return -1;
    }
    return status;
}

int LightEngine::ProjOff()
{
    int status = SetProjectorOnOff(*device0_num, GPIO_IF_NUM, projector_dis);
    if(status!=0)
    {
        qWarning()<<"光机未能关闭";
        return -1;
    }
    CyLibraryExit();
    freegl();
    return status;
}

QString LightEngine::GetVersion()
{
    return VERSION;
}
