#include "Script/HeaterControl/heatercontrol.h"

HeaterControl::HeaterControl(QObject *parent) : QObject(parent)
{
    wiringPiSetup();
    pinMode(HeaterPin,OUTPUT);
    HeaterClock = new QTimer();
    HeaterClock->start(5000);
    connect(HeaterClock,&QTimer::timeout,this,&HeaterControl::ConfigTemp);
}

HeaterControl::~HeaterControl()
{
    delete HeaterClock;
}

void HeaterControl::HeaterOn()
{
    digitalWrite(HeaterPin,HIGH);
    ConfigAndState::SetPrintState("heat");
}

void HeaterControl::HeaterOff()
{
    digitalWrite(HeaterPin,LOW);
    if(ConfigAndState::GetPrintState() == "heat")
    {
        ConfigAndState::SetPrintState("start");
    }
}

void HeaterControl::StopHeater()
{
    digitalWrite(HeaterPin,LOW);
    if(ConfigAndState::GetPrintState() == "heat")
    {
        ConfigAndState::SetPrintState("NULL");
    }
    HeaterClock->stop();
    XMLOperation xmlWrite("../config/realmakerSetting.config");
    ConfigAndState::heaterSwitch = "关";
    if(xmlWrite.WriteXMLData("加热开关","关"))
    {
        qDebug()<<"加热功能关闭";
    }
}

void HeaterControl::OpenHeater()
{
    HeaterClock->start(1000);
    XMLOperation xmlWrite("../config/realmakerSetting.config");
    ConfigAndState::heaterSwitch = "开";
    if(xmlWrite.WriteXMLData("加热开关","开"))
    {
        qDebug()<<"加热功能打开";
    }
}

void HeaterControl::ConfigTemp()
{
    if (ConfigAndState::GetPrintState() == "start" || ConfigAndState::GetPrintState() ==  "continue" || ConfigAndState::GetPrintState() ==  "heat")
    {
        QString temperaturePath = "/sys/bus/w1/devices/";
        QString targetDir = "";
        QDir dir(temperaturePath);
        bool hasFileFlag = false;
        double tempValue = 0;
        foreach (QFileInfo info,dir.entryInfoList())
        {
            if(info.fileName().startsWith("28"))
            {
                targetDir = info.fileName();
            }
        }
        if(targetDir != "")
        {
            QString infoFilePath = QString("%1%2/w1_slave").arg(temperaturePath).arg(targetDir);
            QString TemperatureContent = "";
            QFile temperatureFile(infoFilePath);

            if(!temperatureFile.open(QIODevice::ReadOnly))
            {
                qDebug()<<"无法打开温控文件";
                ConfigAndState::resinTemp = 0;
                return;
            }
            hasFileFlag = true;
            QTextStream fileStream(&temperatureFile);
            //读取文件内容
            TemperatureContent = fileStream.readAll();
            temperatureFile.close();
            tempValue = TemperatureContent.split("\n")[1].split("=")[1].toDouble();
            tempValue = tempValue/1000;
            ConfigAndState::resinTemp = tempValue;
        }
        else
        {
            ConfigAndState::resinTemp = 0;
            return;
        }

        qDebug() << "ConfigAndState::GetPrintState()的状态： " << ConfigAndState::GetPrintState();

        if(hasFileFlag && tempValue < ConfigAndState::targetResinTemp && ConfigAndState::GetPrintState() != "heat")
        {
            qDebug() << "开始加热-目标温度-" << ConfigAndState::targetResinTemp << "-当前温度-" << tempValue;
            HeaterOn();
        }
        else if(hasFileFlag && tempValue > ConfigAndState::targetResinTemp + 5 && ConfigAndState::GetPrintState() == "heat")
        {
            qDebug() << "停止加热-目标温度-" << ConfigAndState::targetResinTemp << "-当前温度-" << tempValue;
            HeaterOff();
        }
        else
        {
            qDebug() << "保持前一状态-目标温度-" << ConfigAndState::targetResinTemp << "-当前温度-" << tempValue;
            //HeaterOff();
        }
        int target = ConfigAndState::heater.split(",")[0].toInt();
        ConfigAndState::heater = QString("%1,%2").arg(QString::number(target)).arg(QString::number(tempValue,10,1));
    }

}
