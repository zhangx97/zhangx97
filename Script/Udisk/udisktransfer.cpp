#include "udisktransfer.h"
#include <QDir>
#include "Script/StateMachine/config_and_state.h"


UdiskTransfer::UdiskTransfer(QObject *parent) : QObject(parent)
{

}

void UdiskTransfer::udiskTranserButton_clicked()
{
    QString devsda1 = "/dev/sda1";
    QFile devsda1file(devsda1);
    if (devsda1file.exists())
    {
        //qDebug() << "成功识别到U盘";
        QString mediaudisk = "/media/udisk";
        QDir mediaudiskDIR(mediaudisk);
        if (!mediaudiskDIR.exists())
        {
            system(QString("sudo mkdir %1").arg(mediaudisk).toUtf8());
        }
        system("sudo mount /dev/sda1 /media/udisk");
        QString RMKID = ConfigAndState::printID;
        //QString udiskpath = "/media/udisk/slicepack/";
        QString udiskpath = QString("/media/udisk/%1/SlicepackPath/").arg(RMKID);
        QString udisksettingfile = QString("/media/udisk/%1/%2_udisksetting.config").arg(RMKID).arg(RMKID);
        qDebug() << "udisksettingfile :" << udisksettingfile;
        QFile udisksettingfileEXIS(udisksettingfile);
        if (udisksettingfileEXIS.exists())
        {
            //qDebug() << "成功识别到配置文件";
            QDir dir(udiskpath);
            QString slicepackpath = "/home/pi/python_project/slicepack/";
            QString unzipslicepackpath = "/home/pi/python_project/unzipSlicepack/";
            QStringList nameFilters;
            nameFilters << "*.slicepack";
            QStringList files = dir.entryList(nameFilters,QDir::Files|QDir::Readable,QDir::Time);

            for (int i = 0; i < files.count(); i++)
            {
                qDebug() << files[i];
                QString slicepackFile = QString("%1%2.zip").arg(slicepackpath).arg(files[i].split(".")[0]);
                QString unzipSlicepackDir = QString("%1%2").arg(unzipslicepackpath).arg(files[i].split(".")[0]);
                QString udiskSlicepackFile = QString("%1%2").arg(udiskpath).arg(files[i]);
                qDebug() << "slicepackFile :" << slicepackFile << "unzipSlicepackDir :" << unzipSlicepackDir << "udiskSlicepackFile :" << udiskSlicepackFile;

                system(QString("sudo rm %1 -rf").arg(slicepackFile).toUtf8());
                qDebug() << "删除slicepack目录下同名.zip :" << QString("sudo rm %1 -rf").arg(slicepackFile);
                system(QString("sudo rm %1 -rf").arg(unzipSlicepackDir).toUtf8());
                qDebug() << "删除unzipSlicepack目录下同名目录 :" << QString("sudo rm %1 -rf").arg(unzipSlicepackDir);
                system(QString("sudo cp %1 %2 -rf").arg(udiskSlicepackFile).arg(slicepackFile).toUtf8());
                qDebug() << "将U盘上的.slicepack复制到slicepack目录中 :" << QString("sudo cp %1 %2 -rf").arg(udiskSlicepackFile).arg(slicepackFile);
                system(QString("sudo unzip -P realmaker123 %1 -d %2").arg(slicepackFile).arg(unzipSlicepackDir).toUtf8());
                system(QString("sudo rm %1/0*.png").arg(unzipSlicepackDir).toUtf8());
                /*---------------------------------判断slicepack文件是否超过最大数量-----------------------------------*/
                ConfigAndState::deleteExtraFile("ZZDB*.zip");
                ConfigAndState::deleteExtraFile("XFMX*.zip");
//                QStringList CurrentAllNameFilters;
//                CurrentAllNameFilters << "*.zip";
//                QStringList CurrentAllFiles = dir.entryList(CurrentAllNameFilters,QDir::Files|QDir::Readable,QDir::Time);
//                int deletenum = CurrentAllFiles.count() - ConfigAndState::slicepackMaxNum;
//                if (deletenum > 0)
//                {
//                    for (int j = 0; j < deletenum; j++)
//                    {
//                        QString deleteSlicepackFile = QString("%1%2.zip").arg(slicepackpath).arg(CurrentAllFiles[CurrentAllFiles.count()-j-1].split(".")[0]);
//                        QString deleteUnzipSlicepackDir = QString("%1%2").arg(unzipslicepackpath).arg(CurrentAllFiles[CurrentAllFiles.count()-j-1].split(".")[0]);
//                        system(QString("sudo rm %1 -rf").arg(deleteSlicepackFile).toUtf8());
//                        qDebug() << "U盘传输slicepack,删除slicepack目录下旧的超过最大文件限制的文件 :" << QString("sudo rm %1 -rf").arg(deleteSlicepackFile);
//                        system(QString("sudo rm %1 -rf").arg(deleteUnzipSlicepackDir).toUtf8());
//                        qDebug() << "U盘传输slicepack,删除slicepack目录下旧的超过最大文件限制的文件夹 :" << QString("sudo rm %1 -rf").arg(deleteUnzipSlicepackDir);
//                    }

//                }
                /*---------------------------------判断slicepack文件是否超过最大数量-----------------------------------*/
            }
            //qDebug() << "从U盘传输文件成功";
            system("sudo umount /media/udisk");
            system("sudo umount /media/pi/REALMAKER");
            emit udiskTransferSuccess(files, QString::number(files.count()));
            return ;
        }
        //qDebug() << "没识别到配置文件或者配置文件不对";
        emit noudisksetting();
        system("sudo umount /media/udisk");
        system("sudo umount /media/pi/REALMAKER");
        return ;
    }
    //qDebug() << "没识别到U盘";
    emit nodevsda1();
    return ;
}
