#ifndef NETWORK_H
#define NETWORK_H
#include <QWidget>
#include <QString>
#include <QUdpSocket>
#include <QTimer>
#include <QThread>

class UDPNetwork : public QObject
{
    Q_OBJECT

public:
    UDPNetwork(QObject *parent = 0);
    void SendUDPBoardcast(int,QString);

private:
    //UDP数据

    QUdpSocket *udpSocket;

};

#endif // NETWORK_H
