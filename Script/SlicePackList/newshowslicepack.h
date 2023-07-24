#ifndef SHOWSLICEPACK_H
#define SHOWSLICEPACK_H

#include <QAbstractListModel>

class ShowSlicePack : public QAbstractListModel
{
    Q_OBJECT
public:
    enum RoleNames {
        FirstNumRole = Qt::UserRole,
        FirstNameRole = Qt::UserRole + 1,
        FirstPathRole = Qt::UserRole + 2,
        FirstSizeRole = Qt::UserRole + 3,
        FirstDateRole = Qt::UserRole + 4,
        SecondNumRole = Qt::UserRole + 5,
        SecondNameRole = Qt::UserRole + 6,
        SecondPathRole = Qt::UserRole + 7,
        SecondSizeRole = Qt::UserRole + 8,
        SecondDateRole = Qt::UserRole + 9
    };

    explicit ShowSlicePack(QObject *parent = nullptr);

    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;

    Q_INVOKABLE void reload(QString reloadmodelname);

protected:
    virtual QHash<int, QByteArray> roleNames() const override;

private:
    void findPack(QString modelname);
    QVector<QVector<QString>*> allPack;
    QHash<int, QByteArray> m_roleNames;

};

#endif // SHOWSLICEPACK_H
