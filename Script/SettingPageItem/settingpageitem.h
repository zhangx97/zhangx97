#ifndef SETTINGPAGEITEM_H
#define SETTINGPAGEITEM_H

#include <QAbstractListModel>
#include "Script/StateMachine/config_and_state.h"

class SettingPageItem:public QAbstractListModel
{
    Q_OBJECT
public:
    SettingPageItem(QObject *parent = nullptr);
    ~SettingPageItem();
    enum SettingItemToQML
    {
        functionNameRole = Qt::UserRole,
        functionStateRole = Qt::UserRole+1
    };
    enum settingItemDetail
    {
        deviceDetail,netState,material,update,endItem
    };

    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index ,int role) const override;
    void InputItem();

protected:
    virtual QHash<int,QByteArray> roleNames() const;

private:
    typedef QVector<QString> Item;
    QVector<Item*> settingList;
    QHash<int,QByteArray> roleNameToQml;

};

#endif // SETTINGPAGEITEM_H
