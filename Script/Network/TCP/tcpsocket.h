#ifndef TCPCLIENTSOCKET_H
#define TCPCLIENTSOCKET_H

#include <QTcpSocket>
#include <QByteArray>
#include <QObject>
#include <QList>
#include <QHostAddress>
#include "Script/Network/TCP/getfile.h"
#include "Script/StateMachine/config_and_state.h"

class TcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    TcpSocket(qintptr socketDescriptor,QObject *parent = 0);
    ~TcpSocket();

    void ReadDataSlot();
    void DisconnectSlot();
    int bytesToInt(QByteArray bytes);
    QString byteToUTF8(QByteArray bytes);
    void PackAnalyze(QByteArray pack);

signals:
    void SendCMDSignal(QByteArray,qintptr);
    void DisconnectSingal(qintptr);

private:
    QString ip;
    qintptr handle;
    qint32 blockSize = 0;
    bool fileMode = false;//文件传输模式
    bool waitForByte = false;//等待数据就绪
    GetFile reciveFile;
    QByteArray filelist;
    int packCounts = 0;
};

#endif // TCPCLIENTSOCKET_H
