#ifndef QMLPRINTCONTROL_H
#define QMLPRINTCONTROL_H
#include <QObject>
#include <QString>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QApplication>
#include <QThread>
#include <QProcess>

#include "Script/Print3DControler/print_control.h"
#include "Script/programcore.h"
#include "Script/Network/HttpUpdate/http_update.h"

class QMLControl:public QObject
{
    Q_OBJECT
public:
    QMLControl();
    ~QMLControl();

    Q_INVOKABLE void printStart(QString slicePackPath);

    ProgramCore *core;

    void PhotoChangeSlot(QString path);
    void UnzipFinishSlot();
    void StopPrintSlot();

    //检查更新
    QThread *DownloadThread;
    Q_INVOKABLE void checkUpdate(QString version);
    Q_INVOKABLE void startUpgrade();
    HTTPUpdate *UpdateSoft;
    Q_INVOKABLE void zipReadySlot();

    //加热开关
    Q_INVOKABLE void heatSwitch(bool change);

signals:
    void QMLPrintStart(QString packName);
    void changePhoto(QString path);
    void unzipFinish();
    void stopPrint();
    void UpdateCheck(QString version);
    void confirmUpdate();
    void changeHeatSwitch(bool flag);

private:
    void InitScreen();  //初始化触摸屏

};

#endif // QMLPRINTCONTROL_H
