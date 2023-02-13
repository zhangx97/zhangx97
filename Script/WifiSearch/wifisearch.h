#ifndef WIFISEARCH_H
#define WIFISEARCH_H

#include <QObject>
#include <QDir>
#include <QDebug>
#include <QRegExp>
#include <QCheckBox>
#include <QTextCodec>
#include <QByteArray>
#include <QHash>
#include <QScroller>
#include <QListWidget>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QAbstractListModel>
#include <QVector>
#include <QProcess>
#include <QTimer>

#include "Script/WifiSearch/wifitable.h"
#include "Script/WifiSearch/wificonnect.h"
#include "Script/Network/NetworkActiveCheck/network_active_check.h"

class WifiSearch : public QAbstractListModel
{
    Q_OBJECT
    //property是整个模型的属性，而role是模型内部一行数据的属性
    Q_PROPERTY(QString Name READ GetName WRITE SetName)
    Q_PROPERTY(QString PassWord READ GetPassword WRITE SetPassword)
    Q_PROPERTY(QString CurrentWifi READ GetCurrentWifi)
    Q_PROPERTY(bool WifiEnable READ GetWifiEnable WRITE SetWifiEnable)//设置wifi
public:
    enum WifiNames//用来将数据传入data
    {
        NameRole = Qt::UserRole,
        QualityRole = Qt::UserRole+1
    };

    NetworkActiveCheck *checkNet;
    QTimer *wifiCheck;
    explicit WifiSearch(QObject *parent = 0);
    ~WifiSearch();

    Q_INVOKABLE void Refresh();    //更新wifi列表
    Q_INVOKABLE void connectWifi();    //连接wifi

    virtual int rowCount(const QModelIndex &parent) const override;     //返回model中数据的条目个数
    virtual QVariant data(const QModelIndex &index , int role) const override;      //获取某一行，某个角色对应的数据
    //获得与设置wifi名
    QString GetName() const;
    void SetName(QString name);
    //获得与设置wifi密码
    QString GetPassword() const;
    void SetPassword(QString password);
    //获得当前的连接wfi
    QString GetCurrentWifi();
    QString ChangeHexToName(QString wifiName);
    //是否允许开启wifi
    bool GetWifiEnable();
    void SetWifiEnable(bool enable);
    void CheckWIFI();

protected:
    //roleNames返回一个哈希表，将role与role-name关联起来，qml返回role-name时，就能够通过role-name查到需要的role,用查到的role来调用data，可以获取到实际数据
    virtual QHash<int,QByteArray> roleNames() const;
signals:
    void netCheckResult(bool success);

private:
    typedef QVector<QString> wifiFind;      //用来存储找到的wifi名和信号质量
    QVector<wifiFind*> allWifi;     //存储所有找到的wifi
    QHash<int ,QByteArray> propertyName;        //提供qml需要的role:role:name对,qml返回role的索引来告诉C++需要哪个role
                                                //role是单个一行的属性
    void SearchWifi();
    void clear();
    void wifiListQuickSort(QVector<wifiFind*> &list ,int begin ,int end);
    bool tempFlag = false;  //是否确立基准数
    QString wifiName;
    QString wifiPassword;
    QString currentWifi;
};

#endif // WIFISEARCH_H
