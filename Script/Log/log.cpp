#include "Script/Log/log.h"

#include <QTime>
#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <QDir>
#include <QFileInfoList>
#include <iostream>
#include <QMutex>

namespace LOG
{
    static QString logFileName;

    void initLogFileName()//初始化日志文件名
    {
        logFileName = QString("../"+logFolderName + "/%1.txt")
                  .arg(QDate::currentDate().toString("yyyyMMdd"));
    }


    void deleteOldLogs()//删除旧的日志，只保留最近60天
    {
        QDir dir;
        dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
        dir.setSorting(QDir::Time | QDir::Reversed);
        dir.setPath(logFolderName);

        QFileInfoList list = dir.entryInfoList();
        if (list.size() <= LOGFILES)
        {
            return;
        } else
        {
            for (int i = 0; i < (list.size() - LOGFILES); i++)
            {
                QString path = list.at(i).absoluteFilePath();
                QFile file(path);
                file.remove();
            }
        }
    }

    bool initLogging()
    {
        //QString fileDate =QString("Log_%1.txt").arg(QDate::currentDate().toString("yyyy_MM_dd"));

        //创建日志文件夹
        if(!QDir(QString("../%1").arg(logFolderName)).exists())
        {
            QDir().mkdir(logFolderName);
        }

        deleteOldLogs();
        initLogFileName();
        //输出日志文件
        QFile outFile(logFileName);
        if(outFile.open(QIODevice::WriteOnly | QIODevice::Append))
        {
            qInstallMessageHandler(myMessageHandler);
            return true;
        }
        else
        {
            return false;
        }
    }

    void myMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString& txt)
    {
        QMutex mutex;
        mutex.lock();
        //检查日志长度，需要的话新建一个
        {
            QFile outFileCheck(logFileName);
            int size = outFileCheck.size();

            if (size > LOGSIZE) //check current log size
            {
                deleteOldLogs();
                initLogFileName();
            }
        }

        QFile outFile(logFileName);
        if (!outFile.open(QIODevice::WriteOnly | QIODevice::Append))
        {
            outFile.close();
            return;
        }

        QString currentDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        QString msg;
        switch (type)
            {
            case QtDebugMsg:
                msg = QString("%1@@Debug@@%2@@%3@@%4@@%5@@")//顺序是：时间 类型 信息 所在文件 所在方法 所在行
                        .arg(currentDateTime).arg(txt).arg(context.file).arg(context.function).arg(context.line);
                break;
            case QtInfoMsg:
                msg = QString("%1@@Info@@%2@@%3@@%4@@%5@@")
                        .arg(currentDateTime).arg(txt).arg(context.file).arg(context.function).arg(context.line);
                break;
            case QtWarningMsg:
                msg = QString("%1@@Warning@@%2@@%3@@%4@@%5@@")
                        .arg(currentDateTime).arg(txt).arg(context.file).arg(context.function).arg(context.line);
                break;
            case QtCriticalMsg:
                msg = QString("%1@@Critical@@%2@@%3@@%4@@%5@@")
                        .arg(currentDateTime).arg(txt).arg(context.file).arg(context.function).arg(context.line);
                break;
            case QtFatalMsg:
                msg = QString("%1@@Fatal@@%2@@%3@@%4@@%5@@")
                        .arg(currentDateTime).arg(txt).arg(context.file).arg(context.function).arg(context.line);
                abort();
                break;
            default:
                break;
            }

        QTextStream ts(&outFile);
        ts << msg << endl;
        mutex.unlock();
    }

}

