#include "qmlstate.h"
#include <QDir>

QMLState::QMLState()
{
    //状态每秒更新
    updateTimer = new QTimer();
    updateTimer->start(5000);
    connect(updateTimer,&QTimer::timeout,[&](){
        UpdateSecond();
    });
}

QMLState::~QMLState()
{
    qDebug()<<"QMLState析构";
    delete updateTimer;
}

void QMLState::pause()
{
    ConfigAndState::SetPrintState("pause");
}

void QMLState::printContinue()
{
    ConfigAndState::SetPrintState("continue");
}

void QMLState::stop()
{
    ConfigAndState::SetPrintState("stop");
}

void QMLState::UpdateSecond()
{
    GetPrintPercent();
    machineSerialID = ConfigAndState::printID;

    GetPrintState();
    GetLayerCount();
    GetFinishCount();
    GetLayerThickness();
    GetRemainTime();
    ReturnNum();

    GetResinTemp();
    GetLEDBrightness();
    GetLEDCurrent();
    GetLEDTemp();
}

double QMLState::GetPrintPercent()
{
    QString percentText = QString::number((nowFinishCount/allLayerCount),10,4);
    percent = percentText.toDouble();
    emit printPercentChanged(percent);
    return percent;
}

void QMLState::SetPauseFlag(bool flag)
{
    pauseFlag = flag;
    emit pauseFlagChanged(pauseFlag);
}

bool QMLState::GetPauseFlag()
{
    return pauseFlag;
}

QString QMLState::GetPrintState()
{
    nowPrintState = ConfigAndState::GetPrintState();
    emit printStateChanged(nowPrintState);
    return nowPrintState;
}

int QMLState::GetLEDBrightness()
{
    ledBright = ConfigAndState::curBrightness;//还是从光机直接读？
    emit ledBrightnessChanged(ledBright);
    return ledBright;
}

double QMLState::GetLEDTemp()
{
    ledTemp = ConfigAndState::curLEDTemp;
    emit ledTempChanged(ledTemp);
    return ledTemp;
}

double QMLState::GetLayerCount()
{
    allLayerCount = (double)(ConfigAndState::modelCount+ConfigAndState::bottomCount);
    emit layerCountChanged(allLayerCount);
    return allLayerCount;
}

int QMLState::GetLEDCurrent()
{
    ledCurrentValue = ConfigAndState::curCurrent;
    emit ledCurrentChanged(ledCurrentValue);
    return ledCurrentValue;
}

double QMLState::GetFinishCount()
{
    nowFinishCount = (double)ConfigAndState::GetFinishCount();
    emit finishCountChanged(nowFinishCount);
    return nowFinishCount;
}

double QMLState::GetLayerThickness()
{
    layThick = ConfigAndState::layerThickness;
    emit layerThicknessChanged(layThick);
    return layThick;
}

double QMLState::GetResinTemp()
{
    resinTemp = ConfigAndState::resinTemp;
    emit resinTempChanged(resinTemp);
    return resinTemp;
}

QString QMLState::GetRemainTime()
{
    remainTime = ConfigAndState::CalculateResidueTime();
    emit remainTimeChanged(remainTime);
    return remainTime;//剩余时间需要转换过
}

QString QMLState::ReturnBackImg()
{
    QString ImgFrontPath = "../images/Preview.png";
    QDir imageDir(ImgFrontPath);
    QString imagePath = "file:///"+imageDir.absolutePath();
    return imagePath;
}

QString QMLState::ReturnFrontImg()
{
    QString ImgFrontPath = "../images/BottomView.png";
    QDir imageDir(ImgFrontPath);
    QString imagePath = "file:///"+imageDir.absolutePath();
    return imagePath;
}

QString QMLState::ReturnTestImg()
{
    QString ImgFrontPath = "../images/BeautifulView.png";
    QDir imageDir(ImgFrontPath);
    QString imagePath = "file:///"+imageDir.absolutePath();
    return imagePath;
}

QString QMLState::ReturnNum()
{
    machineSerialID = ConfigAndState::printID;
    emit printIDChange(machineSerialID);
    return machineSerialID;
}
