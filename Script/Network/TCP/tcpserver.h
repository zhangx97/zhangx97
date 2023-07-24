#ifndef TCPNETWORK_H
#define TCPNETWORK_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QMutex>
#include <QThread>
#include <QList>
//#include "Script/Network/TCP/tcpclientsocket.h"
#include "Script/StateMachine/config_and_state.h"
#include "Script/Network/TCP/tcpsocket.h"
#include "Script/Network/TCP/message_deal.h"

class TcpServerPrivate;
class TCPServer:public QTcpServer
{
    Q_OBJECT
public:
    TCPServer();
    ~TCPServer();

    MessageDeal * ToDealMessage;

//    QTcpSocket *CurrentTcpSocket;     //用于Socket不开新线程的写法
//    TcpSocket *CurrentTcpSocket;

    //void ReadOrder();
    //void DealOrderThenBack(QString);
    void WriteOrderToPCSlot(QByteArray,qintptr);
    void FileTransSlot(QByteArray,qintptr);

//    void NewConnectionSlot();
    void ReadDataSlot(QByteArray,qintptr);
    void DisconnectSlot(qintptr);
    void StartPrintSlot(QString fileName);
    void ContinuePrintSlot();
    void ProjectionIMGSlot(QString imgPath);
    void UTF8ToGB2312(QByteArray &code);
    QThread * DealOrderThread;

signals:
    void StartDealSignal(QString,qintptr);
    void StartPrintSignal(QString filePath);
    void ContinuePrintSignal();
    void ProjectionIMGSignal(QString imgPath);

protected:
    virtual void incomingConnection(qintptr socketDescriptor);

private:
    //QString ip;
    QString order;
    QString messageNeedToSend;
//    ConfigAndState * ConnectToStateMachine;
    QList<TcpSocket *> tcpclientsocketlist;
//    QHash<qintptr,TcpSocket *>tcpDictionary;
    TcpServerPrivate *privateHash;
    QTimer * timer;

    int hisCount = 0;
};

class TcpServerPrivate {
public:
    QMap<qintptr, TcpSocket *> clients; ///所有连接的map
};

#endif // TCPNETWORK_H
