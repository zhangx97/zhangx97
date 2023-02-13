#include "update.h"

Update::Update(QObject *parent)
{
    UpdatePercent = new QTimer();
//    UpdatePercent->start(1000);
    UpdateCheck = new HTTPUpdate();
//    connect(UpdatePercent,&QTimer::timeout,this,&Update::RefreshPercent);
    connect(UpdatePercent,&QTimer::timeout,this,&Update::ShowPercent);
    connect(UpdateCheck,&HTTPUpdate::FindNewVersion,this,&Update::FindNewVersionSlot);
}

Update::~Update()
{
    delete UpdatePercent;
    delete UpdateCheck;
    if(!ConfigRead)
    {
        delete ConfigRead;
    }
}

bool Update::CheckUpgradeStatus()
{
    isUpgrade = ConfigAndState::isInUpdateStatus;
    return isUpgrade;
}

double Update::ShowPercent()
{
    downloadPercent = ConfigAndState::downloadPercent;
    emit percentChange(downloadPercent);
    return downloadPercent;
}

void Update::clickDownload()
{
    UpdatePercent->start(1000);
    ConfigAndState::isInUpdateStatus = true;//开启更新状态,关闭在Network/HTTPUpdate类里面
}

void Update::RefreshPercent()
{
    downloadPercent = ConfigAndState::downloadPercent;
    emit percentChange(downloadPercent);
}

void Update::checkNewVersion()
{
    UpdateCheck->IsLastVersion("http://realmaker.cc/cn/softwareversin3_1.aspx");
}

void Update::FindNewVersionSlot(bool find,QString version)
{
    emit newVersion(find,version);
}

bool Update::IfPackExist()
{
    ConfigRead = new XMLOperation("../config/realmakerSetting.config");
    QString tempBool= ConfigRead->ReadXMLText("更新位");
    if(tempBool == "true")
    {
        isPackExist = true;
    }
    else
    {
        isPackExist = false;
    }
    return isPackExist;
}
