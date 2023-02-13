#ifndef TEMPERATURE_READ_H
#define TEMPERATURE_READ_H

#include <QObject>
#include <QDir>
#include <QStringList>
#include <QFile>
#include <QDebug>
#include <QTextStream>

class TemperatureRead:public QObject
{
    Q_OBJECT
public:
    QString temperatureDir;
    QString ReadTemperature();

private:
    void GetTemperaDir();

};

#endif // TEMPERATURE_READ_H
