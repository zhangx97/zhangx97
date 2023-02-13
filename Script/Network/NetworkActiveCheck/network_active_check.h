#ifndef NETWORK_ACTIVE_CHECK
#define NETWORK_ACTIVE_CHECK

#include <QWidget>
#include <QLabel>
#include <QFile>
#include <QHostInfo>
#include <QNetworkInterface>
#include <QTimer>
#include <QDir>
#include <QProcess>
#include <stdlib.h>

#include "Script/StateMachine/config_and_state.h"

class NetworkActiveCheck:public QObject
{
    Q_OBJECT

public:
    NetworkActiveCheck();
    ~NetworkActiveCheck();
    void CheckNetworkOnline();
    void ChangeIPAndRoute();
    void GetRoute();
    void GetConnectState();
    void NetworkTest();
    bool InternetTest();
    bool LocalNetTest();

private:
    QString currentIP;
    bool addCutFlag = true;        //ip增减标志，true+  false-
    QString routeIP;
    QString connectState;
    bool localNetAlive;
    bool internetAlive;
};

#endif
