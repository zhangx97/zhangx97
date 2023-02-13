#ifndef HEATERCONTROL_H
#define HEATERCONTROL_H

#include <QObject>
#include <QTimer>
#include <QFileInfo>
#include <QDir>
#include <QFile>
#include <QTextStream>

#include "wiringPi.h"
#include "Script/StateMachine/config_and_state.h"
#include "Script/Print3DControler/xml_operation.h"

#define HeaterPin 28
class HeaterControl : public QObject
{
    Q_OBJECT
public:
    explicit HeaterControl(QObject *parent = nullptr);
    ~HeaterControl();

    void StopHeater();
    void OpenHeater();
    void HeaterOn();
    void HeaterOff();
    void ConfigTemp();
    
    QTimer *HeaterClock;

signals:

public slots:

};

#endif // HEATERCONTROL_H
