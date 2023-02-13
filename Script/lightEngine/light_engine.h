#ifndef LIGHT_ENGINE_H
#define LIGHT_ENGINE_H

#include <QString>
#include <QLibrary>
extern "C"
{

}

class LightEngine
{
public:
    LightEngine(int);
    char * maskdir = "/home/pi/python_project/mask/";
    QLibrary * cyusb;
    int lightSensorType;
    int * device0_num = 0;
    int projector_en = 1;
    int projector_dis = 0;
    QString lightengineSN = "NULL";
    bool readyState = false;
    bool firstWithoutAdjust = true;
    QString VERSION;
    
    //调用库中的方法                                                    //为了避免麻烦，每个函数都定义一个函数指针
    typedef int (*CyAPIInit)(int lightSensorType);
    typedef int (*CySetProjectorOnOff)(int deviceNumber, int interfaceNum, int PorjectorEnable);
    typedef int (*CySetLedOnOff)(int deviceNumber, int interfaceNum, int LedEnableRed, int LedEnableGreen, int LedEnableBlue);
    typedef int (*CyGetLedOnOff)(int deviceNumber, int interfaceNum, int *LedEnableRed, int *LedEnableGreen, int *LedEnableBlue);
    typedef int (*CySetLedCurrent)(int deviceNumber, int interfaceNum, int currentRed, int currentGreen, int currentBlue);
    typedef int (*CyGetLedCurrent)(int deviceNumber, int interfaceNum, int *currentRed, int *currentGreen, int *currentBlue);
    typedef int (*CyFlashBlockRead)(int deviceNumber, int interfaceNum, int address, int len, char *buf);
    typedef int (*CyGetTemperature)(int deviceNumber, int interfaceNum, int *tempValue);
    typedef int (*CyGetLight)(int deviceNumber, int interfaceNum, int *lightValue);
    typedef int (*CySetTestPattern)(int deviceNumber, int interfaceNum, uint8_t PatternSel);
    typedef int (*CyFlashBlockReadMask)(char *str,char *lightengineSN);
    typedef int (*CyCyLibraryExit)();
    typedef int (*Cyfreegl)();
    //typedef int *CyDEVICE0_NUM;

    
    CyAPIInit APIInit;
    CySetProjectorOnOff SetProjectorOnOff;
    CySetLedOnOff SetLedOnOff;
    CyGetLedOnOff GetLedOnOff;
    CySetLedCurrent SetLedCurrent;
    CyGetLedCurrent GetLedCurrent;
    CyFlashBlockRead FlashBlockRead;
    CyGetTemperature GetTemperature;
    CyGetLight GetLight;
    CySetTestPattern SetTestPattern;
    CyFlashBlockReadMask FlashBlockReadMask;
    CyCyLibraryExit CyLibraryExit;
    Cyfreegl freegl;
    //CyDEVICE0_NUM device0_num;

    //
    int InitLED();
    int IsMaskExist();      //用不到
    int ReadFile();     //用不到
    int ReadLEDState();
    int LEDOn();
    int LEDOff();
    int SetLightValue(int targetLight,int offset);
    int AutoSetLightValue(int targetLight,int offset);
    int LightValueFinalCalibrate(int targetLight,int offset);       //没有必要继续使用
    int WriteLEDCurrentValue(int currentValue);
    int ReadLEDCurrentValue();
    int ReadLEDSensorValue();
    int ReadLEDTemp();
    int ImageFlipSetting();     //用不到
    int ProjOn();       //还有未完成
    int ProjOff();      //还有未完成
    QString GetVersion();

private:
    int I2C_IF_NUM = 0;
    int SPI_IF_NUM = 1;
    int GPIO_IF_NUM = 2;

    int ADDR_SNUM = 0;
    int ADDR_LEDC = 43;
    int ADDR_LGHT = 51;
    int ADDR_MASK = 65536;

    
};
#endif // LIGHT_ENGINE_H
