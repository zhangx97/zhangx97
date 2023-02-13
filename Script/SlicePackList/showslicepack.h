#ifndef SHOWSLICEPACK_H
#define SHOWSLICEPACK_H
#include <QAbstractListModel>
#include <QVector>
#include "Script/Print3DControler/xml_operation.h"

class ShowSlicePack:public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString packName READ packName WRITE getPackName)
    Q_PROPERTY(QString packPath READ packPath WRITE getPackPath)
    Q_PROPERTY(QString material READ getMaterial)
    Q_PROPERTY(QString thickness READ getThickness)
    Q_PROPERTY(QString curingTime READ getCuringTime)

public:
    enum slicePackName
    {
        NameRole = Qt::UserRole,
        PathRole = Qt::UserRole+1
    };

    ShowSlicePack(QObject *parent = 0);
    ~ShowSlicePack();

    Q_INVOKABLE QString returnFrontImg();
    Q_INVOKABLE QString returnBackImg();
    Q_INVOKABLE void initInfo();

    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    void getPackName(QString getSlicePackName);
    void getPackPath(QString getSlicePackPath);
    void reload();
    QString packName();
    QString packPath();

    QString getMaterial();//获取当前选中包的材料
    QString getThickness();//获取当前选中包的厚度
    QString getCuringTime();//获取当前选中包的固化时间

    QString modelMaterial;
    QString modelThickness;
    QString modelCuringTime;

private:
    virtual QHash<int,QByteArray> roleNames() const;
//    QVector<QString> packInfo;
    QVector<QVector<QString>*> allPack;
    QHash<int,QByteArray> packProperty;
    QString nameOfPack = "";
    QString pathOfPack = "";
    QString packInfoPath = "";
    XMLOperation *packInfoInit;

    void findPack();
    void QuickSort(QVector<QString> *list ,int begin ,int end);
};

#endif // SHOWSLICEPACK_H
