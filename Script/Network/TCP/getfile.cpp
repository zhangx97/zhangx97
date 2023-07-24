#include "getfile.h"
#include <QDir>
#include <QDebug>
#include <QIODevice>
#include <QDataStream>
#include <stdlib.h>
#include "Script/StateMachine/config_and_state.h"

GetFile::GetFile()
{

}

void GetFile::save(QByteArray &fileData)
{
    qDebug()<<"fileName : " << fileName;
    QDir fileToSave(QString("../slicepack/%1").arg(fileName));
    QString filePath = fileToSave.absolutePath();
    qDebug()<<"slicepack保存路径："<<filePath;

    QFile saveFile(filePath);
    if(!saveFile.open(QIODevice::WriteOnly))
    {
        qDebug()<<"保存silcepack失败";
    }
    else
    {
        QDataStream fileStream(&saveFile);
        fileStream.writeRawData(fileData,fileData.size());
    }
    saveFile.close();
    ConfigAndState::SetSliceStatus(true);

    QString unzipFileNoSuffix = "../unzipSlicepack/"+fileName.split(".")[0];
    qDebug()<<"创建文件夹的位置："<<unzipFileNoSuffix;
    QDir makeDir(unzipFileNoSuffix);
    makeDir.mkdir(unzipFileNoSuffix);
    QString dirpath = makeDir.absolutePath();
    qDebug()<<"解压文件夹："<<dirpath;
    system(QString("sudo unzip -P realmaker123 %1 -d %2").arg(filePath).arg(dirpath).toUtf8());
//    system(QString("sudo rm -r %1").arg(filePath).toUtf8());

    fileName = "";
    fileCreateDate = "";
    packSize = 0;
    packCount = 0;
    lastPackSize = 0;
}
