#include "Script/Network/HttpUpdate/http_update.h"

HTTPUpdate::HTTPUpdate():accessManager(new QNetworkAccessManager)
{


}

HTTPUpdate::~HTTPUpdate()
{
    qDebug()<<"HTTPUpdate开始析构";
    if(XMLOperate)
    {
        delete XMLOperate;
        XMLOperate = nullptr;
    }
    qDebug()<<"网络响应类析构完毕";
    if(file)
    {
        qDebug()<<"file指针有内容";
        if(file->isOpen())
        {
            file->close();
        }
        file->deleteLater();
        file = nullptr;
    }
    if(ReplyZip)
    {
        qDebug()<<"ReplyZip";
        ReplyZip->deleteLater();
        ReplyZip = nullptr;
    }
    if(ReplyVersion)
    {
        qDebug()<<"replyversion";
        ReplyVersion->deleteLater();
        ReplyVersion = nullptr;
    }
    if(accessManager)
    {
        qDebug()<<"accessManager";
        accessManager->disconnect();
        accessManager->deleteLater();
        accessManager = nullptr;
    }
    qDebug()<<"HTTPUpdate析构结束";
}

void HTTPUpdate::IsLastVersion(QString urlToCheck)
{
    qDebug()<<"当前网络线程ID:"<<QThread::currentThreadId()<<"\n";
    QUrl url(urlToCheck);
    QNetworkRequest requestN(url);
    requestN.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
    ReplyVersion = accessManager->get(requestN);
    qDebug()<<"收到网站返回信息";

    connect(ReplyVersion,&QNetworkReply::readyRead,this,&HTTPUpdate::AnalyseVersionSlot);
    connect(accessManager,&QNetworkAccessManager::finished,this,&HTTPUpdate::NetManagerFinished);
    connect(ReplyVersion,&QNetworkReply::finished,this,&HTTPUpdate::ReplyVersionFinishedSlot);
    connect(ReplyVersion,QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),    //这个写法是官方文档的
            this,&HTTPUpdate::LoadErrorSlot);
}

void HTTPUpdate::NetManagerFinished()
{
    accessManager->disconnect();
    accessManager->deleteLater();
    accessManager = nullptr;
}

void HTTPUpdate::Download(QString version)//内部的连接以后需要修改过
{
    lastVersion = version;

    file = new QFile(QString("../update/RealmakerV%1.zip").arg(lastVersion));
    qDebug()<<"存放路径:"<<QString("../update/RealmakerV%1.zip").arg(lastVersion);
    file->open(QIODevice::WriteOnly);
    QString urlToDownload = QString("http://realmaker.cc/cn/RealmakerV%1.zip").arg(lastVersion);
//        QString urlToDownload = QString("http://realmaker.cc/cn/softwareversin3_1.aspx");   //测试时使用
    qDebug()<<"下载链接:"<<urlToDownload;
    QUrl url(urlToDownload);
    QNetworkRequest requestZ(url);
    requestZ.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
    ReplyZip = accessManager->get(requestZ);

    connect(ReplyZip,&QNetworkReply::readyRead,this,&HTTPUpdate::DownloadZip);
//        connect(accessManager,&QNetworkAccessManager::finished,this,&HTTPUpdate::ReplyZipFinishedSlot);
    connect(ReplyZip,&QNetworkReply::finished,this,&HTTPUpdate::ReplyZipFinishedSlot);
    connect(ReplyZip,QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),    //这个写法是官方文档的
            this,&HTTPUpdate::LoadErrorSlot);
    connect(ReplyZip, &QNetworkReply::downloadProgress, this, &HTTPUpdate::CalculatePercent);
}

void HTTPUpdate::AnalyseVersionSlot()
{
    qDebug()<<"开始解析html";
    QString content = ReplyVersion->readAll();
    QRegExp rg("(?:\\D+=\"last\">)(\\d+.\\d+.\\d+)(?:<\/div>)");
    int posLast = rg.indexIn(content);
    lastVersion = rg.cap(1);
    qDebug()<<"当前最新版本："<<rg.cap(1);


    QStringList localVersion = ConfigAndState::version.split('.');
    QStringList netVersion = lastVersion.split('.');
    qDebug()<<lastVersion;

    if(localVersion[0].toInt() <= netVersion[0].toInt())
    {
        if(localVersion[0].toInt()<netVersion[0].toInt())
        {
            qDebug()<<"需要更新";
            needUpdate = true;
//            Download();
            emit FindNewVersion(needUpdate,lastVersion);
            return;
        }
        else
        {
            if (localVersion[1].toInt() < netVersion[1].toInt())
            {
                qDebug()<<"需要更新";
                needUpdate = true;
//                Download();
                emit FindNewVersion(needUpdate,lastVersion);
                return;
            }else if (localVersion[1].toInt() == netVersion[1].toInt())
            {
                if (localVersion[2].toInt() < netVersion[2].toInt())
                {
                    qDebug()<<"需要更新";
                    needUpdate = true;
//                    Download();
                    emit FindNewVersion(needUpdate,lastVersion);
                    return;
                }
            }
        }
    }
    qDebug()<<"不必更新";
    needUpdate = false;
    emit FindNewVersion(needUpdate,lastVersion);
}

void HTTPUpdate::ReplyZipFinishedSlot()
{
    qDebug()<<"网络连接结束";
    if(ReplyZip->error() == QNetworkReply::NoError)
    {
        ReplyZip->deleteLater();
        ReplyZip = nullptr;
        file->flush();
        file->close();
    }
    ConfigAndState::isInUpdateStatus = false;//关闭更新状态,开启在Update类里面
    ConfigAndState::downloadPercent = 1.0;
    XMLOperate = new XMLOperation("../config/realmakerSetting.config");
    XMLOperate->CreateXML();
    XMLOperate->WriteXMLData("更新位","true");
    emit UpgradeZipReady();
}

void HTTPUpdate::RefreshSoft()
{
    QProcess *pPrecess = new QProcess(this);
    pPrecess->startDetached("../update/update");
    QThread::sleep(1);

    QApplication* app;
    app->exit(0);
}

void HTTPUpdate::ReplyVersionFinishedSlot()
{
    qDebug()<<"网络1连接结束";
    ReplyVersion->deleteLater();
    ReplyVersion = nullptr;
}

void HTTPUpdate::LoadErrorSlot(QNetworkReply::NetworkError)
{
    qDebug()<<"发生网络故障";

    if(ReplyZip)
    {
        qDebug()<<"Error:"<<ReplyZip->error();
        ReplyZip->deleteLater();
        ReplyZip = nullptr;
    }
    if(ReplyVersion)
    {
        qDebug()<<"Error:"<<ReplyVersion->error();
        ReplyVersion->deleteLater();
        ReplyVersion = nullptr;
    }
}

void HTTPUpdate::DownloadZip()
{
    file->write(ReplyZip->readAll());
}

void HTTPUpdate::CalculatePercent(qint64 bytesSent, qint64 bytesTotal)
{
//    qDebug()<<bytesSent;
    double percent = (double)bytesSent/(double)bytesTotal;
    QString percentText = QString::number(percent,10,4);
//    qDebug()<<"进度"<<percentText;
    ConfigAndState::downloadPercent = percentText.toDouble();
}
