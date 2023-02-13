#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include "Script/StateMachine/config_and_state.h"
#include "Script/Print3DControler/xml_operation.h"

class SerialPort
{
public:
    SerialPort();
    QString ArduinoPortName;
//    QStringList GetPortNameList();
    bool OpenPort(QString);
    bool ReceiveInfo(QByteArray message = "");
    bool SendPort(QByteArray);
    bool SerialPortInit();
//    QString ChosePort();

    ~SerialPort();

private:
    QSerialPort * serialPort;
    QStringList portNameList;
    bool serialIsOpen;
};

#endif // SERIALPORT_H
