#include "Script/Print3DControler/temperature_read.h"

QString TemperatureRead::ReadTemperature()
{
    GetTemperaDir();
    QString temperaFile;
    QStringList temperaFileList;
    QFile readTempera(temperatureDir);
    if(!readTempera.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"读取温度文件出错";
    }
    else
    {
        QTextStream ReadFile(&readTempera);
        temperaFile = ReadFile.readAll();
        temperaFileList = temperaFile.split("\n");
        for(int i=0;i<temperaFileList.count();i++)
        {
            if(temperaFileList.at(i).contains("t="))
            {
                return temperaFileList.at(i).split("=")[1];
            }
        }
    }
}

void TemperatureRead::GetTemperaDir()
{
    QDir path("/sys/bus/w1/devices/");
    QStringList fileList = path.entryList();
    for(int i=0;i<fileList.count();i++)
    {
        QString tempFileName = fileList.at(i).split("-")[0];
        if(tempFileName == "28")
        {
            temperatureDir = QString("/sys/bus/w1/devices/%1").arg(fileList.at(i));
        }
    }
}
