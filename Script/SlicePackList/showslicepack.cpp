#include "showslicepack.h"
#include <QDir>
#include <QRegExp>
#include <QDebug>
#include <QTimer>
#include "Script/StateMachine/config_and_state.h"

ShowSlicePack::ShowSlicePack(QObject* parent):QAbstractListModel (parent)
{
    packProperty[NameRole] = "packNameCpp";
    packProperty[PathRole] = "slicePathCpp";
    QTimer *sliceFind = new QTimer();
    sliceFind->start(500);
    connect(sliceFind,&QTimer::timeout,[&](){
        if(ConfigAndState::GetSliceStatus())
        {
            reload();
            ConfigAndState::SetSliceStatus(false);
        }
    });
    findPack();
    nameOfPack = (*allPack[0])[0];
    pathOfPack = (*allPack[0])[1];
    initInfo();
}

ShowSlicePack::~ShowSlicePack()
{
    allPack.clear();
}

QString ShowSlicePack::packName()
{
    return nameOfPack;
}

void ShowSlicePack::getPackName(QString getSlicePackName)
{
    nameOfPack = getSlicePackName;
}

QString ShowSlicePack::packPath()
{
    return pathOfPack;
}

void ShowSlicePack::getPackPath(QString getSlicePackPath)
{
    pathOfPack = getSlicePackPath;
}

int ShowSlicePack::rowCount(const QModelIndex &parent) const
{
    return allPack.size();
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

QHash<int,QByteArray> ShowSlicePack::roleNames() const
{
    return packProperty;
}

void ShowSlicePack::findPack()
{
    qDebug()<<"初始化slicepack包";
    QString path = "../slicepack/";
    QDir dir(path);
    QStringList nameFilters;//
    QString head;//获取正则匹配的头部
    QString foot;//获取正则匹配的尾部
    nameFilters <<"*.zip";
    QStringList files = dir.entryList(nameFilters,QDir::Files|QDir::Readable,QDir::Time);
    QVector<QString> *filesAfterCut = new QVector<QString>();//存储文件日期
    QVector<QString> *packInfo;
    for (int i = 0 ;i<files.count();i++) {
        packInfo = new QVector<QString>();
        packInfo->append(files[i].split(".")[0]);
        packInfo->append(QString("../slicepack/%1").arg(files[i]));
        allPack.append(packInfo);
        packInfo = 0;
    }

//    qDebug()<<"slicepack开始匹配正则";
//    //遍历所有文件名,用正则匹配到日期,然后按日期排序,排序后分配
//    for(int i = 0; i<files.count();i++)
//    {
//        QRegExp rx("(\\d{9})");
//        qDebug()<<"开始正则"<<files[i];
//        rx.setMinimal(false);
//        int pos = 0;

//        rx.indexIn(files[i],pos);
//        qDebug()<<"匹配到的串："<<rx.cap(0);
//        QString body = rx.cap(0);
//        filesAfterCut->append(body);
//    }
//    //排序
//    QuickSort(filesAfterCut,0,filesAfterCut->count()-1);
//    //依照排序，重新分配files列表中的值
//    QVector<QString> *packInfo;
//    for (int i =0;i<filesAfterCut->count();i++)
//    {
//        for(int j =0;j<files.count();j++)
//        {
//            if(files[j].contains((*filesAfterCut)[i]))
//            {
//                packInfo = new QVector<QString>();
//                QString slicePathName = files[j].split(".zip")[0];
//                packInfo->append(slicePathName);
//                packInfo->append(QString("../slicepack/%1.zip").arg(slicePathName));
//                allPack.append(packInfo);
//                packInfo = 0;
//            }
//        }
//    }


}

void ShowSlicePack::reload()
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
    findPack();
    endResetModel();
}

void ShowSlicePack::QuickSort(QVector<QString> *list, int begin, int end)
{

    if(begin<end)
    {
        int tempValue = (*list)[begin].toInt();//保存基准元素
        int i = begin;
        int j = end;
        while(i<j)
        {
            //j值小于基准值时，左移一位
            while(i<j && (((*list)[j].toInt())<tempValue))
            {
                j--;
            }

            //大于基准值的j，与i位置交换
            (*list)[i] = (*list)[j];
            //i值大于基准值时，右移一位
            while(i<j && (((*list)[i].toInt())>=tempValue))
            {
                i++;
            }
            //i值小于基准值时，与j位置交换
            (*list)[j] = (*list)[i];
        }
        //将值填入中间区域
        (*list)[i] =QString::number(tempValue);
        //对左右两侧的剩余区域进行排序
        QuickSort(list,begin,i-1);
        QuickSort(list,i+1,end);
    }
    else
    {
        return;
    }
}

QString ShowSlicePack::returnFrontImg()
{
    QString ImgFrontPath = "../unzipSlicepack/"+nameOfPack+"/Preview.png";
    QDir imageDir(ImgFrontPath);
    QString imagePath = "file:///"+imageDir.absolutePath();
    return imagePath;
}

QString ShowSlicePack::returnBackImg()
{
    QString ImgBackPath = "../unzipSlicepack/"+nameOfPack+"/BottomView.png";
    QDir imageDir(ImgBackPath);
    QString imagePath = "file:///"+imageDir.absolutePath();
    return imagePath;
}

void ShowSlicePack::initInfo()
{
   QString slicepackPath =  "../unzipSlicepack/"+nameOfPack+"/model.pack";
   packInfoInit = new XMLOperation(slicepackPath);
   modelMaterial = packInfoInit->ReadXMLText("material");
   modelThickness = packInfoInit->ReadXMLText("thick");
   modelCuringTime = packInfoInit->ReadXMLText("layer_expose_time");
}

QString ShowSlicePack::getMaterial()
{
    return modelMaterial;
}

QString ShowSlicePack::getThickness()
{
    return modelThickness;
}

QString ShowSlicePack::getCuringTime()
{
    return modelCuringTime;
}
