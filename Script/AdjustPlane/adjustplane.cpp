#include "adjustplane.h"
#include "scrollbar.h"
#include "Script/Network/SerialPort/serialport.h"

AdjustPlane::AdjustPlane(QObject *parent) :
    QObject(parent)
{
    wiringPiSetup();
    pinMode(horizon_pin,INPUT);

    timer = new QTimer();
    timer->start(1000);  //等硬件到了再开起来用
    connect(timer,&QTimer::timeout,this,&AdjustPlane::CheckVoltage);

        printState = ConfigAndState::GetPrintState();
        if(printState == "NULL" || printState == "stop")
        {
            horizonSerial = new SerialPort();
            horizonSerial->SendPort("M18\n");
            horizonSerial->SendPort("G91\n");
        }
    position = 50;
    emit positionChange(QString::number(position,10,2));
}

AdjustPlane::~AdjustPlane()
{
    delete horizonSerial;
    delete timer;
}

void AdjustPlane::CalculatePosition(int value)
{
    position+=value;
}

void AdjustPlane::sendCmd(QString type)
{
    qDebug()<<"点击调整水平";
    QString cmd = type;
    QStringList cmdSplit = cmd.split(" ");
    if(cmdSplit[0] == "G28")
    {
        position = 0;
    }
    if(cmdSplit[0] == "M18")
    {
        position = 50;
    }
    if(cmdSplit[0] == "G1")
    {
        int x = 0;
        QString positionData = cmdSplit[1].split("Z")[1];
        position+=positionData.toDouble();
    }
    QByteArray send = cmd.toLatin1()+"\n";
    horizonSerial->SendPort(send);
    emit positionChange(QString::number(position,10,2));
}

bool AdjustPlane::CheckVoltage()
{

    printState = ConfigAndState::GetPrintState();

    if(printState == "NULL" || printState == "stop")
    {
        int level = digitalRead(horizon_pin);
        if(level == 1)
        {
            if(VoltageLow)
            {
                VoltageLow = false;
                emit voltageChange(VoltageLow);
            }
        }
        else
        {
            if(!VoltageLow)
            {
                VoltageLow = true;
                emit voltageChange(VoltageLow);
            }
        }
    }

    return VoltageLow;
}

QString AdjustPlane::GetCurrentPosition()
{
    return QString::number(position,10,2);
}
