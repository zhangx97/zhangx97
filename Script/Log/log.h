#ifndef LOG_H
#define LOG_H

#define LOGSIZE 1024 * 1024 //每天1mb的量，应该是完全够用了
#define LOGFILES 360

#include <QObject>
#include <QString>
#include <QDebug>
#include <QDate>
#include <QTime>


namespace LOG
{
const QString logFolderName = "logs";

bool initLogging();
void myMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString& msg);

}

#endif // LOGUTILS_H
