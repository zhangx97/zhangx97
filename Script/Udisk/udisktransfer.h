#ifndef UDISKTRANSFER_H
#define UDISKTRANSFER_H

#include <QObject>
#include <QDebug>

class UdiskTransfer : public QObject
{
    Q_OBJECT
public:
    explicit UdiskTransfer(QObject *parent = nullptr);

    Q_INVOKABLE void udiskTranserButton_clicked();

signals:
    void nodevsda1();
    void noudisksetting();
    void udiskTransferSuccess(QStringList files, QString count);

public slots:
};

#endif // UDISKTRANSFER_H
