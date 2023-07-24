#ifndef READMODELPACK_H
#define READMODELPACK_H

#include <QObject>

#include "Script/StateMachine/config_and_state.h"

class ReadModelPack : public QObject
{
    Q_OBJECT
public:
    explicit ReadModelPack(QObject *parent = nullptr);

    const QString getNowSlicepackPath() const;
    void setNowSlicepackPath(const QString &value);

    const QString getNowMaterial() const;
    void setNowMaterial(const QString &value);

    const QString getNowThick() const;
    void setNowThick(const QString &value);

    const QString getNowCount() const;
    void setNowCount(const QString &value);

    const QString getNowWeight() const;
    void setNowWeight(const QString &value);

    const QString getNowDuration() const;
    void setNowDuration(const QString &value);

    Q_INVOKABLE void getModelPackInof(QString slicepackname);

    Q_INVOKABLE QString getPng(QString slicepackName, QString pngName);

signals:
    void nowSlicepackPathChanged();
    void nowMaterialChanged();
    void nowThickChanged();
    void nowCountChanged();
    void nowWeightChanged();
    void nowDurationChanged();

private:
    QString nowSlicepackPath;
    Q_PROPERTY(QString slicepackPath READ getNowSlicepackPath WRITE setNowSlicepackPath NOTIFY nowSlicepackPathChanged)

    QString nowMaterial;
    Q_PROPERTY(QString material READ getNowMaterial WRITE setNowMaterial NOTIFY nowMaterialChanged)

    QString nowThick;
    Q_PROPERTY(QString thick READ getNowThick WRITE setNowThick NOTIFY nowThickChanged)

    QString nowCount;
    Q_PROPERTY(QString count READ getNowCount WRITE setNowCount NOTIFY nowCountChanged)

    QString nowWeight;
    Q_PROPERTY(QString weight READ getNowWeight WRITE setNowWeight NOTIFY nowWeightChanged)

    QString nowDuration;
    Q_PROPERTY(QString duration READ getNowDuration WRITE setNowDuration NOTIFY nowDurationChanged)



};

#endif // READMODELPACK_H
