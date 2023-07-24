#include "newshowslicepack.h"
#include <QDir>
#include <QDebug>
#include <QCoreApplication>
#include <QFileInfo>
#include <QDateTime>
#include "Script/StateMachine/config_and_state.h"

ShowSlicePack::ShowSlicePack(QObject *parent) : QAbstractListModel(parent)
{
    m_roleNames[FirstNumRole] = "FirstNum";
    m_roleNames[FirstNameRole] = "FirstName";
    m_roleNames[FirstPathRole] = "FirstPath";
    m_roleNames[FirstSizeRole] = "FirstSize";
    m_roleNames[FirstDateRole] = "FirstDate";
    m_roleNames[SecondNumRole] = "SecondNum";
    m_roleNames[SecondNameRole] = "SecondName";
    m_roleNames[SecondPathRole] = "SecondPath";
    m_roleNames[SecondSizeRole] = "SecondSize";
    m_roleNames[SecondDateRole] = "SecondDate";
    findPack("ZZDB*.zip");
}

int ShowSlicePack::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return allPack.count();
}

QVariant ShowSlicePack::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if(row<0 || row >allPack.size())
    {
        return  QVariant();
    }
    QVector<QString> *temp = allPack[index.row()];
    return temp->at(role - Qt::UserRole);
}

QHash<int, QByteArray> ShowSlicePack::roleNames() const
{
    return m_roleNames;
}

void ShowSlicePack::findPack(QString modelname)
{
    qDebug()<<"初始化slicepack包";
    QDir::setCurrent("/home/pi/python_project/core");
    QString path = "../slicepack/";
    /*---------------------------------判断slicepack文件是否超过最大数量-----------------------------------*/
    ConfigAndState::deleteExtraFile("ZZDB*.zip");
    ConfigAndState::deleteExtraFile("XFMX*.zip");
//    QStringList CurrentAllNameFilters;
//    CurrentAllNameFilters << "*.zip";
//    QDir slicepackdir("../slicepack/");
//    QStringList CurrentAllFiles = slicepackdir.entryList(CurrentAllNameFilters,QDir::Files|QDir::Readable,QDir::Time);
//    int deletenum = CurrentAllFiles.count() - ConfigAndState::slicepackMaxNum;
//    if (deletenum > 0)
//    {
//        for (int j = 0; j < deletenum; j++)
//        {
//            QString deleteSlicepackFile = QString("../slicepack/%1.zip").arg(CurrentAllFiles[CurrentAllFiles.count()-j-1].split(".")[0]);
//            QString deleteUnzipSlicepackDir = QString("../unzipSlicepack/%1").arg(CurrentAllFiles[CurrentAllFiles.count()-j-1].split(".")[0]);
//            system(QString("sudo rm %1 -rf").arg(deleteSlicepackFile).toUtf8());
//            qDebug() << "电脑端发送slicepack,删除slicepack目录下旧的超过最大文件限制的文件 :" << QString("sudo rm %1 -rf").arg(deleteSlicepackFile);
//            system(QString("sudo rm %1 -rf").arg(deleteUnzipSlicepackDir).toUtf8());
//            qDebug() << "电脑端发送slicepack,删除slicepack目录下旧的超过最大文件限制的文件夹 :" << QString("sudo rm %1 -rf").arg(deleteUnzipSlicepackDir);
//        }

//    }
    /*---------------------------------判断slicepack文件是否超过最大数量-----------------------------------*/
    QDir dir(path);
    QStringList nameFilters;
    QString head;//获取正则匹配的头部
    QString foot;//获取正则匹配的尾部
    nameFilters << modelname;  // "*.zip"  QStringList可以通过append()，或使用<< 来添加List元素
    QStringList files = dir.entryList(nameFilters,QDir::Files|QDir::Readable,QDir::Time);
    //QFileInfo info(QString("../slicepack/%1").arg(files[1]));
    QVector<QString> *packInfo; //使用QVector可以预先不定义大小
    qDebug()<<"slicepack文件个数： " << files.count();
    for (int i = 0 ; i < files.count(); i = i + 2) {
        packInfo = new QVector<QString>();
        for (int j = 0 ; j < 2 ; j++) {
            if ( i + j == files.count() )
            {
                packInfo->append(QString::number(i+j+1, 10));
                packInfo->append("more");
                packInfo->append("2");
                packInfo->append("--");
                packInfo->append("----/--/--");
            } else {
                packInfo->append(QString::number(i+j+1, 10));
                packInfo->append(files[i+j].split(".")[0]);

                QString filepath = "/home/pi/python_project/slicepack/" + files[i+j];
                qDebug() << "filepath  --  " << filepath;
                QFileInfo fileinfo(filepath);
                double filesize = fileinfo.size();
                qDebug() << "文件大小" << QString::number(filesize/1024/1024, 'f', 2);

                qDebug() << "文件日期birthTime" << fileinfo.birthTime().toString("yyyy/MM/dd");
                qDebug() << "文件日期lastModified" << fileinfo.lastModified().toString("yyyy/MM/dd");
                qDebug() << "文件日期metadataChangeTime" << fileinfo.metadataChangeTime().toString("yyyy/MM/dd");
                qDebug() << "文件日期lastRead" << fileinfo.lastRead().toString("yyyy/MM/dd");

                packInfo->append("2");
                packInfo->append(QString::number(filesize/1024/1024, 'f', 2));
                packInfo->append(fileinfo.lastModified().toString("yyyy/MM/dd"));
            }
        }
        qDebug()<<*packInfo;
        allPack.append(packInfo);
        packInfo = nullptr;

    }
    qDebug()<<allPack;

}

void ShowSlicePack::reload(QString reloadmodelname)
{
    beginResetModel();
    int count = allPack.count();
    if(count>0)
    {
        for (int i =0;i<count;i++)
        {
            delete allPack.at(i);
        }
        allPack.clear();
    }
    findPack(reloadmodelname);
    endResetModel();
}
