#ifndef CYUSBSERIALCONTROLLER_H
#define CYUSBSERIALCONTROLLER_H

#include <QString>
extern "C"{
    #include "Script/lightEngine/CyUSBSerial.h"
}

class CyUSBSerialController
{
public:
    CyUSBSerialController();

    CY_RETURN_STATUS cyAPIInit(const quint8 &deviceNum,int lightSensorType);
    CY_RETURN_STATUS setProjectorOnOff(const quint8 &deviceNum, const quint8 &enable);
    CY_RETURN_STATUS resetCypressDevice(const quint8 &deviceNum);

protected:
    QString errorString;
    qint32 findDevice();
    CY_RETURN_STATUS getLedOnOff(const quint8 &deviceNum, quint16 &ledRed, quint16 &ledGreen, quint16 &ledBlue);
    CY_RETURN_STATUS setLedOnOff(const quint8 &deviceNum, quint16 &ledRed, quint16 &ledGreen, quint16 &ledBlue);
    CY_RETURN_STATUS setLedCurrent(const quint8 &deviceNum, quint16 &ledRed, quint16 &ledGreen, quint16 &ledBlue);
    CY_RETURN_STATUS getLedCurrent(const quint8 &deviceNum, qint32 &ledRed, qint32 &ledGreen, qint32 &ledBlue);
    CY_RETURN_STATUS flashBlockRead(const quint8 &deviceNum, quint32 address, quint32 len, char *buf);
    CY_RETURN_STATUS setTestPattern(const quint8 &deviceNum, const quint8 &PatternSel);
    CY_RETURN_STATUS getLightSensor(const quint8 &deviceNum, qint32 &lightValue);
    CY_RETURN_STATUS getLedTemperature(const quint8 &deviceNum, qint32 &tempValue);
    CY_RETURN_STATUS setProjectorFlip(const quint8 &deviceNum, const quint8 &projectFlip);

private:
    CY_RETURN_STATUS GpioWrite(const quint8 &deviceNum, const quint8 &interfaceNum, const quint8 &gpioNum, quint8 value);
    CY_RETURN_STATUS GpioRead(const quint8 &deviceNum,const quint8 &interfaceNum,const quint8 &gpioNum,quint8 *value);
    CY_RETURN_STATUS I2CWrite(const quint8 &deviceNum, const quint8 &interfaceNum, const quint8 &address, const quint32 &len, quint8 *buf);
    CY_RETURN_STATUS I2CRead(const quint8 &deviceNum, const quint8 &interfaceNum, const quint8 &address, const quint32 &len, quint8 *buf);
    CY_RETURN_STATUS cySPIWaitForIdle(const CY_HANDLE &handle);
    CY_RETURN_STATUS isCypressDevice(const quint8 &deviceNum) const;

    quint8 numDevices;
    CY_HANDLE cyHandle;
    CY_RETURN_STATUS rStatus;

    static const int CY_MAX_DEVICES = 30;
    static const int CY_MAX_INTERFACES = 4;
    static const int I2C_IF_NUM = 0;
    static const int SPI_IF_NUM = 1;
    static const int GPIO_IF_NUM = 2;//0、1版光机project on off的gpio脚
    //static const quint16 cSlaveAddress7bit = 0x1B;      //光机slave地址
    //static const quint16 aSlaveAddress7bit = 0x29;      //光传感器slave地址,大功率0x29，普通大概是0x39
    quint16 cSlaveAddress7bit = 0x1B;
    quint16 aSlaveAddress7bit = 0x29;
    quint16 readLightValueCommand = 0xB4;        //大功率0xB4,普通0xAC
};

#endif // CYUSBSERIALCONTROLLER_H
