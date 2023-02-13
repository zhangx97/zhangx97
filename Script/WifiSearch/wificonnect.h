#ifndef WIFICONNECT_H
#define WIFICONNECT_H

#include <QWidget>
#include <QTime>

#include "Script/Network/NetworkActiveCheck/network_active_check.h"
#include "Script/StateMachine/config_and_state.h"

namespace Ui {
class wifiConnect;
}

class wifiConnect : public QWidget
{
    Q_OBJECT

public:
    explicit wifiConnect(QString connectMode,QWidget *parent = nullptr,int custom = 0);
    ~wifiConnect();
    QString wifiPassword;
    void setWifiName(QString);

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private slots:
    void on_cancel_clicked();

    void on_connectWifi_clicked();

private:
    Ui::wifiConnect *ui;
    void CallKeyBoard(QWidget *);
    bool keyboardShow = false;
    QString connectMode;
    void Delay_MSec(unsigned int msec);
};

#endif // WIFICONNECT_H
