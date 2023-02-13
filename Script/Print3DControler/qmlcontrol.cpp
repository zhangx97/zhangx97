#include "qmlcontrol.h"
#include "Script/StateMachine/config_and_state.h"

QMLControl::QMLControl()
{
    core = new ProgramCore();
    InitScreen();
    connect(this,&QMLControl::QMLPrintStart,core,&ProgramCore::QMLStartPrintSlot);
    connect(core,&ProgramCore::QMLUnzipFinish,this,&QMLControl::UnzipFinishSlot);
    connect(core,&ProgramCore::PhotoChanged,this,&QMLControl::PhotoChangeSlot);
    connect(core,&ProgramCore::StopPrint,this,&QMLControl::StopPrintSlot);
    connect(this,&QMLControl::changeHeatSwitch,core,&ProgramCore::HeatSwitchChange);
}

QMLControl::~QMLControl()
{
    qDebug()<<"QMLControl析构";
    delete core;
    delete DownloadThread;
}

void QMLControl::printStart(QString slicePackPath)
{
    emit QMLPrintStart(slicePackPath);
}

void QMLControl::PhotoChangeSlot(QString path)
{
    emit changePhoto(path);
}

void QMLControl::UnzipFinishSlot()
{
    emit unzipFinish();
}

void QMLControl::StopPrintSlot()
{
    emit stopPrint();
}

void QMLControl::checkUpdate(QString version)
{
    UpdateSoft = new HTTPUpdate();
    connect(this,&QMLControl::UpdateCheck,UpdateSoft,&HTTPUpdate::Download);
    connect(UpdateSoft,&HTTPUpdate::UpgradeZipReady,this,&QMLControl::zipReadySlot);
    qDebug()<<"QMLControl收到的下载版本"<<version;
    emit UpdateCheck(version);
}

void QMLControl::zipReadySlot()
{
    emit confirmUpdate();
}

void QMLControl::startUpgrade()
{
    UpdateSoft->RefreshSoft();
}

void QMLControl::heatSwitch(bool change)
{
    if(change)
    {
        emit changeHeatSwitch(true);
    }
    else
    {
        emit changeHeatSwitch(false);
    }
}

void QMLControl::InitScreen()
{
    QString cmdResult = "";
    QStringList cmdFirstAnalyze;//执行结果的第一次切分
    QStringList cmdSecondAnalyze;//再次切分
    QProcess process;
    QString id = "";

    process.start("sudo xinput");
    process.waitForFinished();
    cmdResult = process.readAllStandardOutput();
    cmdFirstAnalyze = cmdResult.split("\n");
    for (int i = 0;i<cmdFirstAnalyze.count();i++)
    {
        cmdSecondAnalyze = cmdFirstAnalyze[i].split("\t",QString::SkipEmptyParts);
        if(cmdSecondAnalyze[0].contains("wch.cn USB2IIC_CTP_CONTROL"))//优创想的屏幕(创乐博的主板)
        {
            id = cmdSecondAnalyze[1].split("id=")[1];
            break;
        }
    }
    if(id != "")
    {
        process.start(QString("xinput map-to-output %1 HDMI-2").arg(id));
        process.waitForFinished();
    }
}
