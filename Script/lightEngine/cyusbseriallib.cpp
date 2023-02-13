#include "Script/lightEngine/cyusbseriallib.h"

#include <QDebug>
#include <QImage>
#include <QThread>
#include <math.h>

CyUSBSerialLib::CyUSBSerialLib(int projectionType):lightSensorType(projectionType)
{
    GetDeviceNumber();
}

CyUSBSerialLib::~CyUSBSerialLib()
{
}

void CyUSBSerialLib::ErrorStatus(CY_RETURN_STATUS status)
{
//    ErrorStatus<RMK_STATUS_TYPES, CY_RETURN_STATUS> err(LIGHT_ERROR_STATUS, status);
//    errorString = err.getErrorStatus();
//    LOG_CRIT << "CY ERROR NO: " << err;
}

bool CyUSBSerialLib::SetProjectorOnOff(bool enable)
{
    if(enable){
        return IsValid(cyAPIInit(devNum,lightSensorType));
    }else {
        return IsValid(CyUSBSerialController::setProjectorOnOff(devNum, enable));
    }
}

void CyUSBSerialLib::SetDeviceNumber(const quint8 &devNum)
{
    this->devNum = devNum;
}

qint32 CyUSBSerialLib::GetDeviceNumber()
{
    qint32 num = findDevice();
    qDebug()<<QString("获取到的光机串口号：%1").arg(num);
    if(num != -1){
        devNum = static_cast<quint8>(num);
        return num;
    }
    return -1;
}

bool CyUSBSerialLib::DeviceInit()
{
    CY_RETURN_STATUS rStatus;
    rStatus = cyAPIInit(devNum,lightSensorType);
    if(rStatus == CY_SUCCESS){
        qDebug()<<"光机初始化成功";
        return true;
    }
    //errorStatus(rStatus);
    return false;
}

inline bool CyUSBSerialLib::IsValid(CY_RETURN_STATUS status) const
{
    return status == CY_SUCCESS ? true : false;
}

bool CyUSBSerialLib::SetLedCurrent(const quint16 &value)
{
    quint16 currentR = value;
    quint16 currentG = value;
    quint16 currentB = value;

    qDebug()<<QString("通入LED电流：%1").arg(value);
    QThread::sleep(1);
    CY_RETURN_STATUS status = CyUSBSerialController::setLedCurrent(devNum, currentR, currentG, currentB);
    if(status != CY_SUCCESS) qDebug()<<"LED电流未能成功设置";       //errorStatus(status);
    return IsValid(status);
}

int CyUSBSerialLib::GetLedCurrent()
{
    qint32 currentR ;
    qint32 currentG ;
    qint32 currentB ;

    CY_RETURN_STATUS status = CyUSBSerialController::getLedCurrent(devNum, currentR, currentG, currentB);
    if(status == CY_SUCCESS){
        return currentG;
    }
    qDebug()<<"LED电流未能获取";      //errorStatus(status);
    return -1;
}

QString CyUSBSerialLib::GetErrorString() const
{
    return errorString;
}

bool CyUSBSerialLib::SetLedOnOff(bool enable)
{
    CY_RETURN_STATUS rStatus;
    quint16 r=0,g=0,b=0;
    if(enable){
        g = 1;
        rStatus = CyUSBSerialController::setLedOnOff(devNum,r,g,b);
    }else {
        rStatus = CyUSBSerialController::setLedOnOff(devNum,r,g,b);
    }
    if(rStatus != CY_SUCCESS) qDebug()<<"未能成功设置LED开关";      //errorStatus(rStatus);
    return IsValid(rStatus);
}

int CyUSBSerialLib::GetLedOnOff()
{
    CY_RETURN_STATUS rStatus;
    quint16 r=0, g=0,b=0;
    rStatus = CyUSBSerialController::getLedOnOff(devNum, r, g, b);
    if(rStatus != CY_SUCCESS){
        qDebug()<<"LED开关状态未能获取";
        //errorStatus(rStatus);
        return -1;
    }else {
        return g;
    }
}

int CyUSBSerialLib::GetLightSensorValue()
{
    qint32 value;
    CY_RETURN_STATUS status = CyUSBSerialController::getLightSensor(devNum, value);
    if(status != CY_SUCCESS){
        return -1;
    }
    return value;
}

double CyUSBSerialLib::GetLedTemperature()
{
    qint32 value;
    CY_RETURN_STATUS status = CyUSBSerialController::getLedTemperature(devNum, value);
    if(status != CY_SUCCESS){
        qDebug()<<"无法取得LED温度";
        //errorStatus(status);
        return -1;
    }
    return value / 10.0;
}

QByteArray CyUSBSerialLib::GetSerialNumber()
{
    CY_RETURN_STATUS rStatus;

    char buffer[19];
    rStatus = flashBlockRead(devNum, 0, 19, buffer);
    if(rStatus != CY_SUCCESS){
        qDebug()<<"无法取得光机序列号";
        //errorStatus(rStatus);
        return nullptr;
    }

    QByteArray array;
    for(quint8 byte : buffer) array.push_back(byte);

    return array;
}

bool CyUSBSerialLib::FlashBlockReadMask(char *fileName)
{
    CY_RETURN_STATUS rStatus;
    qint32 pngLen = 0, pngSum = 0;

    //Read mask file
    char headerBuf[8];
    rStatus = flashBlockRead(devNum, ADDR_MASK, 8, headerBuf);
    if(rStatus != CY_SUCCESS){
        return false;
    }

    for(quint32 i = 0 ; i < 4; i++){
        pngLen |= ((headerBuf[i] << (i*8)) & (0xff << (i*8)));
        pngSum |= ((headerBuf[i+4] << (i*8)) & (0xff << (i*8)));
    }

    char *pngBuf = new char[static_cast<quint64>(pngLen)];
    rStatus = flashBlockRead(devNum, ADDR_MASK + 8, static_cast<quint32>(pngLen),  pngBuf);     //读取数据
    if(rStatus != CY_SUCCESS){
        qDebug()<<"flashBlockReadMask err";
        //errorStatus(rStatus);
        delete [] pngBuf;
        return false;
    }

    qint32 ckSum = 0;
    for(int i = 0; i < pngLen; i++){
        ckSum += pngBuf[i] & 0xff;
    }

    if(ckSum == 0 || ckSum != pngSum){
        delete [] pngBuf;
        return false;
    }

    //FILE *file = fopen(fileName, "wb");
    QByteArray byteArray(pngBuf, pngLen);
    QImage image;
    image.loadFromData(byteArray, "png");
    image.save(fileName);


    //fwrite(pngBuf, 1, static_cast<quint64>(pngLen), file);
    //fclose(file);

    delete [] pngBuf;
    //delete file;

    return true;
}


bool CyUSBSerialLib::SetProjectorFlip(const quint8 &projectFlip)
{
    CY_RETURN_STATUS status = CyUSBSerialController::setProjectorFlip(devNum, projectFlip);
    if(status != CY_SUCCESS) qDebug()<<"setProjectorFlip err";      //errorStatus(status);
    return IsValid(status);
}

bool CyUSBSerialLib::AutoSetValue(int targetLight, int offset)
{
   SetLedOnOff(true);

   //获取序列号与遮罩图片============================================
   ConfigAndState::lightengineSN = GetSerialNumber();
   QByteArray serialNum = ConfigAndState::lightengineSN;
   QString maskPath = QString("../mask/%1.png").arg(serialNum.data());
   ConfigAndState::mask_img = maskPath;
   QFileInfo file(maskPath);
   if(file.exists()==false)
   {
       FlashBlockReadMask(maskPath.toUtf8().data());
   }
   //==============================================================

   SetLedCurrent(100);

   int setNum = 0;
   while (setNum<10)
   {
       int lightValue = GetLightSensorValue();
       int ledCurrent = GetLedCurrent();
       qDebug()<<QString("通入电流：%1；当前亮度：%2；目标亮度：%3").arg(GetLedCurrent())
                                                              .arg(lightValue)
                                                              .arg(targetLight);
       if(abs(lightValue - targetLight)>offset)
       {
           int needCurrentValue = (ledCurrent/lightValue)*targetLight;
           SetLedCurrent(needCurrentValue);
       }else
       {
           return true;
       }
   }
   qWarning()<<"光机未能达到目标亮度";
   return false;
}
