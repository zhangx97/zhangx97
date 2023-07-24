#ifndef CYUSBSERIALLIB_H
#define CYUSBSERIALLIB_H
#include <QFileInfo>

#include "Script/lightEngine/cyusbserialcontroller.h"
#include "Script/StateMachine/config_and_state.h"
#include "Script/Print3DControler/xml_operation.h"

class CyUSBSerialLib : public CyUSBSerialController
{
public:
    explicit CyUSBSerialLib(int);
    ~CyUSBSerialLib();

    bool DeviceInit();
    bool FlashBlockReadMask(char *fileName);

    bool SetLedOnOff(bool enable);
    bool SetProjectorOnOff(bool enable);
    void SetDeviceNumber(const quint8 &devNum);
    bool SetLedCurrent(const quint16 &value);
    bool SetProjectorFlip(const quint8 &projectFlip);

    bool AutoSetValue(int targetLight,int offset);
    //打开光机

    XMLOperation * devNumXML;

    double GetLedTemperature();
    int GetLedCurrent();
    int GetLedOnOff();
    int GetLightSensorValue();
    qint32 GetDeviceNumber();
    QString GetErrorString() const;
    QByteArray GetSerialNumber();

private:
    void ErrorStatus(CY_RETURN_STATUS status);
    bool IsValid(CY_RETURN_STATUS status) const;
    int lightSensorType;

    quint8 devNum;

    static const quint8 ADDR_SNUM = 0;         //Serial number : 19 bytes
    static const quint8 ADDR_LEDC = 43;        //LED current   : 4 bytes
    static const quint8 ADDR_LGHT = 51;        //Light sensor value at dark frame : 4bytes
    static const quint32 ADDR_MASK = 0x10000;  //mask file start address
};

#endif // CYUSBSERIALLIB_H
