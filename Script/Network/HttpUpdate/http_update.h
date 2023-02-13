#ifndef HTTP_UPDATE_H
#define HTTP_UPDATE_H

#include <QWidget>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QFile>
#include <QDebug>
#include <QRegExp>
#include <QStringList>
#include <QProgressBar>
#include <QApplication>

#include "Script/StateMachine/config_and_state.h"
#include "Script/Print3DControler/xml_operation.h"

class HTTPUpdate:public QObject
{
    Q_OBJECT
public:
    HTTPUpdate();
    ~HTTPUpdate();
    void IsLastVersion(QString urlToCheck);
    void Download(QString version);
    void RefreshSoft();
    QFile *file = nullptr;

    bool needUpdate = false;

signals:
    void LoadProgressSignal(qint64 bytesSent,qint64 bytesTotal);
    void UpgradeZipReady();
    void FindNewVersion(bool find,QString version);
private:
    QString lastVersion;    //最新版本号
    QNetworkReply *ReplyVersion = nullptr;       //http连接响应类
    QNetworkReply *ReplyZip = nullptr;       //http连接响应类
    QNetworkAccessManager *accessManager = nullptr;   //http连接管理类
    XMLOperation *XMLOperate = nullptr;

    void DownloadZip();     //单独开一个线程下载

    void ReadContentSlot();
    void ReplyZipFinishedSlot();
    void ReplyVersionFinishedSlot();
    void LoadErrorSlot(QNetworkReply::NetworkError);
    void CalculatePercent(qint64 bytesSent,qint64 bytesTotal);
    void AnalyseVersionSlot();  //判断是否需要更新

    void NetManagerFinished();
};

#endif // HTTP_UPDATE_H
