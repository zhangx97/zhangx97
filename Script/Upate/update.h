#ifndef UPDATE_H
#define UPDATE_H
/*
 * 下载不在这里，为了后台下载考虑，下载放在Network/HttpUpdate里
*/
//#include <QAbstractListModel>
#include <QString>
#include <QTimer>
#include "Script/Network/HttpUpdate/http_update.h"
#include "Script/StateMachine/config_and_state.h"
#include "Script/Print3DControler/xml_operation.h"

class Update:public QObject
{
    Q_OBJECT
public:
    Update(QObject *parent = nullptr);
    ~Update();

    Q_PROPERTY(double percent READ ShowPercent NOTIFY percentChange)
    Q_PROPERTY(bool upgradeStatus READ CheckUpgradeStatus)
    Q_PROPERTY(bool packExist READ IfPackExist)
    Q_INVOKABLE void checkNewVersion();
    Q_INVOKABLE void clickDownload();

    double ShowPercent();
    void RefreshPercent();
    void FindNewVersionSlot(bool find,QString version);
    bool CheckUpgradeStatus();
    bool IfPackExist();

    HTTPUpdate *UpdateCheck;
    XMLOperation *ConfigRead;
    double downloadPercent;
    QTimer *UpdatePercent;
    bool isUpgrade;
    bool isPackExist;
signals:
    void percentChange(double percent);
    void newVersion(bool find,QString version);
};

#endif // UPDATE_H
