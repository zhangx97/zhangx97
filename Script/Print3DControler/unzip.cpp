#include "Script/Print3DControler/unzip.h"
#include <QFileInfo>
#include <QFile>
#include <QDebug>
#include <stdlib.h>
#include <exception>
#include <QDir>
#include <QDirIterator>


bool Unzip::UzipSuccess = false;

Unzip::Unzip()
{

}

Unzip::~Unzip()
{
    delete zipProcess;
}

void Unzip::UnzipImgFile(QString filePath,QString targetPath,bool needPassword)
{
    QFile zipFile(filePath);
    if(!zipFile.exists())
    {
        qDebug()<<"未收到需要解压的打印文件";
        return;
    }

    QDir dir("../images");
    if(dir.isEmpty())
    {
        qDebug()<<"已经清空";
    }
    else
    {
        QDirIterator dirIterator(dir.absolutePath(),
                                 QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot,
                                 QDirIterator::NoIteratorFlags);
        while(dirIterator.hasNext())
        {
            if(dir.remove(dirIterator.next()))
            {
                QDir(dirIterator.filePath()).removeRecursively();
            }
        }
        qDebug()<<"文件夹清空完毕";
    }

    int unzipFlag;
    try
    {
        if(needPassword)
        {
            QString zipOrder =QString("unzip -P realmaker123 %1 -d  %2").arg(filePath).arg(targetPath);
            unzipFlag = system(zipOrder.toUtf8());       //该命令在linux上用
        }
        else
        {
            QString zipOrder =QString("unzip -o %1 -d  %2").arg(filePath).arg(targetPath);
            unzipFlag = system(zipOrder.toUtf8());       //该命令在linux上用
        }
    }
    catch (std::exception& e )
    {
        qDebug()<<"系统解压期间出错:"<<e.what();
    }


    if(unzipFlag == 0)
    {
        Unzip::UzipSuccess = true;
        qDebug()<<"解压成功。";
    }else
    {
        Unzip::UzipSuccess = false;
    }
}

/*
void Unzip::ReadOutput()
{
    qDebug()<<"5555555555555";
    qDebug()<<zipProcess->readAllStandardOutput();
}*/
