#ifndef QMLNETMODEL_H
#define QMLNETMODEL_H

#include <QAbstractListModel>
#include <QTimer>

#include "Script/StateMachine/config_and_state.h"
#include "Script/Network/NetworkActiveCheck/network_active_check.h"

class qmlNetModel:public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(bool localNetActive READ GetLocalActive)
    Q_PROPERTY(bool internetActive READ GetOuterActive)
public:
    enum netState{
        localNet = Qt::UserRole,
        internetNet = Qt::UserRole+1,
        endline
    };

    QTimer *CheckNet;

    qmlNetModel(QObject *parent = nullptr);
    bool GetLocalActive();
    bool GetOuterActive();
    virtual int rowCount(const QModelIndex &parent) const override;     //返回model中数据的条目个数
    virtual QVariant data(const QModelIndex &index , int role) const override;      //获取某一行，某个角色对应的数据
    void qmlNetStateInit();

protected:
    virtual QHash<int,QByteArray> roleNames() const;

private:
    QHash<int,QByteArray> qmlNetRoleName;
    QVector<bool> qmlNetState;
};

#endif // QMLNETMODEL_H
