#include "readmodelpack.h"
#include <QDebug>

#include "Script/Print3DControler/xml_operation.h"
#include <QDir>

ReadModelPack::ReadModelPack(QObject *parent) : QObject(parent)
{
//    qDebug() << nowSlicepackPath;
//    getModelPackInof(nowSlicepackPath);
}

const QString ReadModelPack::getNowSlicepackPath() const
{
    return nowSlicepackPath;
}

void ReadModelPack::setNowSlicepackPath(const QString &newNowSlicepackPath)
{
    if (nowSlicepackPath == newNowSlicepackPath)
        return;
    nowSlicepackPath = newNowSlicepackPath;
    emit nowSlicepackPathChanged();
}

const QString ReadModelPack::getNowMaterial() const
{
    return nowMaterial;
}

void ReadModelPack::setNowMaterial(const QString &value)
{
//    if (nowMaterial == value)
//        return;
    nowMaterial = value;
    emit nowMaterialChanged();
}

const QString ReadModelPack::getNowThick() const
{
    return nowThick;
}

void ReadModelPack::setNowThick(const QString &value)
{
    if (nowThick == value)
        return;
    nowThick = value;
    emit nowMaterialChanged();
}

const QString ReadModelPack::getNowCount() const
{
    return nowCount;
}

void ReadModelPack::setNowCount(const QString &value)
{
    if (nowCount == value)
        return;
    nowCount = value;
    emit nowCountChanged();
}

const QString ReadModelPack::getNowWeight() const
{
    return nowWeight;
}

void ReadModelPack::setNowWeight(const QString &value)
{
    if (nowWeight == value)
        return;
    nowWeight = value;
    emit nowWeightChanged();
}

const QString ReadModelPack::getNowDuration() const
{
//    nowDuration = ConfigAndState::CalculateResidueTime();
    return nowDuration;
}

//QString QMLState::GetRemainTime()
//{
//    remainTime = ConfigAndState::CalculateResidueTime();
//    emit remainTimeChanged(remainTime);
//    return remainTime;//剩余时间需要转换过
//}

void ReadModelPack::setNowDuration(const QString &value)
{
    if (nowDuration == value)
        return;
    nowDuration = value;
    emit nowDurationChanged();
}

void ReadModelPack::getModelPackInof(QString slicepackname)
{
    QString modelPackPath = "/home/pi/python_project/unzipSlicepack/" + slicepackname + "/model.pack";
    qDebug() << modelPackPath;
    XMLOperation * modelPackInfoGet = new XMLOperation(modelPackPath);
    nowMaterial = modelPackInfoGet->ReadXMLText("material");
    nowThick = modelPackInfoGet->ReadXMLText("thick");
    nowCount = modelPackInfoGet->ReadXMLText("count");
    bool ok;
    double volume = modelPackInfoGet->ReadXMLText("volume").toDouble(&ok);
    nowWeight = QString::number(volume * 1.3, 'f', 2);
//    nowDuration = ConfigAndState::CalculateResidueTime();
    qDebug() << "nowMaterial" << nowMaterial;
    qDebug() << "nowThick" << nowThick;
    qDebug() << "nowCount" << nowCount;
    qDebug() << "nowWeight" << nowWeight;
    qDebug() << "volume" << volume;

    double motorMoveTime = (double)ConfigAndState::motorMoveTime/1000;
    double seconds = 0;
    double tempCuring = modelPackInfoGet->ReadXMLText("layer_expose_time").toDouble();
    seconds += motorMoveTime * nowCount.toInt() * tempCuring;
    int int_s = (int)seconds;
    int h = int_s/(60*60);
    int m =(int_s-(h*60*60))/60;
    int sec = (int_s-(h*60*60))-m*60;
    QString hour = QString::number(h);
    if(hour.length()==1) hour = "0"+hour;
    QString minute = QString::number(m);
    if(minute.length()==1) minute = "0"+minute;
    QString second = QString::number(sec);
    if(second.length()==1) second = "0"+second;
    nowDuration = hour+":"+minute+":"+second;
    qDebug() << "nowDuration" << nowDuration;
}

QString ReadModelPack::getPng(QString slicepackName, QString pngName)
{
    QString pngPath = "/home/pi/python_project/unzipSlicepack/" + slicepackName + "/" + pngName;
    QDir pngDir(pngPath);
    QString pngLocalPath = "file:///"+pngDir.absolutePath();
    return pngLocalPath;
}




//void ReadModelPack::qDebugNowSlicepackPathChanged()
//{
//    qDebug() << "nowSlicepackPath -- NOW" << nowSlicepackPath;
//}
