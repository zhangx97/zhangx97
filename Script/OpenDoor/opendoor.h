#ifndef OPENDOOR_H
#define OPENDOOR_H

#include <QObject>
#include <QProcess>

class OpenDoor : public QObject
{
    Q_OBJECT
public:
    explicit OpenDoor(QObject *parent = nullptr);

    Q_INVOKABLE void dooropenbutton_clicked();

signals:

public slots:

private:
    QProcess process;

};

#endif // OPENDOOR_H
