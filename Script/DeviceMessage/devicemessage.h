#ifndef DEVICEMESSAGE_H
#define DEVICEMESSAGE_H

#include <QWidget>
#include <QAbstractListModel>

#include "Script/WifiSearch/wifisearch.h"
#include "Script/StateMachine/config_and_state.h"

class DeviceMessage : public QAbstractListModel
{
    Q_OBJECT

public:
    enum{
        infoName = Qt::UserRole,
        infoContent = Qt::UserRole+1,
        infoSection = Qt::UserRole+2
    };
    enum{
        rmkid,rmkType,LEDID,LEDType,ip,systemVersion,sleepTime,heatSwitch,endline
    };

    explicit DeviceMessage(QObject *parent = nullptr);
    ~DeviceMessage();
    Q_INVOKABLE void refresh();
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index ,int role) const override;
    void GetDeviceInfo();

protected:
    virtual QHash<int,QByteArray> roleNames() const;

private:
    QHash<int,QByteArray> roleName;
    typedef QVector<QString> item;
    QVector<item*> deviceInfo;
    void clear();
};

#endif // DEVICEMESSAGE_H
