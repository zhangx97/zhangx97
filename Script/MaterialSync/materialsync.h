#ifndef MATERIALSYNC_H
#define MATERIALSYNC_H

#include <QAbstractListModel>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVector>

#include "Script/Print3DControler/xml_operation.h"
#include "Script/SQL/sql.h"
#include "Script/StateMachine/config_and_state.h"

class MaterialSync:public QAbstractListModel
{
    Q_OBJECT
public:
    enum materialItem{
        NameRole = Qt::UserRole,
        LightRole = Qt::UserRole+1
    };
    Q_INVOKABLE void startSync();
    Q_INVOKABLE void updateMaterial();

    MaterialSync(QObject *parent = nullptr);
    ~MaterialSync();
    SQL *MaterialSQL;

    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index ,int role) const override;

protected:
    virtual QHash<int,QByteArray> roleNames() const;

private:
//    typedef QVector<QString> signalMaterial;
    QVector<QVector<QString> *> materialList;
    QHash<int,QByteArray> roleName;
    XMLOperation *materialXml;
    QString RMKID="";
    QSqlQuery result;
    QVector<QString> materialNameList;//需要同步的模型列表

    int GDLX = 0;
    int DWQ = 0;
    int LSY = 0;
    int XFMX = 0;
    int ZZMX = 0;
    int ZZDB = 0;
    int ZJLX = 0;
    int RZYY = 0;

    void Getmaterial();
    void GetGDLX();
    void GetDWQ();
    void GetLSY();
    void GetXFMX();
    void GetZZMX();
    void GetZZDB();
    void GetZJLX();
    void GetRZYY();

};

#endif // MATERIALSYNC_H
