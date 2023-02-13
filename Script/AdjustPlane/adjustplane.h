#ifndef ADJUSTPLANE_H
#define ADJUSTPLANE_H

#include <QObject>
#include <QString>
#include <QTimer>

#include "Script/Network/SerialPort/serialport.h"
#include "wiringPi.h"
#include "Script/StateMachine/config_and_state.h"

#define horizon_pin 21

class AdjustPlane : public QObject
{
    Q_OBJECT

public:
    explicit AdjustPlane(QObject *parent = nullptr);
    ~AdjustPlane();
    Q_INVOKABLE void CalculatePosition(int value);//计算当前位置
    Q_INVOKABLE void sendCmd(QString type);//向串口发送指令
    Q_INVOKABLE bool CheckVoltage();//检测电平变化
    Q_PROPERTY(QString CurrentPosition READ GetCurrentPosition NOTIFY positionChange)
    Q_PROPERTY(bool voltage READ CheckVoltage NOTIFY voltageChange)

    SerialPort *horizonSerial;
    QTimer *timer;

    double position;
    bool VoltageLow = false;
    QString GetCurrentPosition();//获取当前位置

signals:
    void positionChange(QString position);
    void voltageChange(bool changed);

private:
    QString printState = "";
};

#endif // ADJUSTPLANE_H
