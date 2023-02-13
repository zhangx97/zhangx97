#include "Script/Network/TCP/tcpserver.h"
#include <stdlib.h>

TCPServer::TCPServer():ToDealMessage(new MessageDeal),privateHash(new TcpServerPrivate)
{
//    listen(QHostAddress::Any,ConfigAndState::port);

    connect(this,&TCPServer::StartDealSignal,ToDealMessage,&MessageDeal::DealOrder);
    connect(ToDealMessage,&MessageDeal::OrderResultSignal,this,&TCPServer::WriteOrderToPCSlot);
    connect(ToDealMessage,&MessageDeal::StartPrintSignal,this,&TCPServer::StartPrintSlot);
    connect(ToDealMessage,&MessageDeal::ContinuePrintSignal,this,&TCPServer::ContinuePrintSlot);
    connect(ToDealMessage,&MessageDeal::ProjectionIMG,this,&TCPServer::ProjectionIMGSlot);
    connect(ToDealMessage,&MessageDeal::FileTransSignal,this,&TCPServer::FileTransSlot);
}

TCPServer::~TCPServer()
{
    for (TcpSocket *tcp : privateHash->clients.values())
    {
        tcp->disconnectFromHost();
        auto handle = tcp->socketDescriptor();
        tcp->deleteLater();
        DisconnectSlot(handle);
    }
    close();

    delete ToDealMessage;
    delete timer;
    delete privateHash;

}

void TCPServer::incomingConnection(qintptr socketDescriptor)
{
    TcpSocket *CurrentTcpSocket = new TcpSocket(socketDescriptor);         //当前tcp指向新加入的连接
    connect(CurrentTcpSocket,&TcpSocket::SendCMDSignal,this,&TCPServer::ReadDataSlot);
    connect(CurrentTcpSocket,&TcpSocket::DisconnectSingal,this,&TCPServer::DisconnectSlot);
    Q_ASSERT(CurrentTcpSocket->socketDescriptor() == socketDescriptor);
    privateHash->clients.insert(socketDescriptor,CurrentTcpSocket);
    WriteOrderToPCSlot("Welcome!",socketDescriptor);
}

void TCPServer::ReadDataSlot(QByteArray buffer,qintptr descriptor)
{
    emit StartDealSignal(buffer,descriptor);
}

void TCPServer::WriteOrderToPCSlot(QByteArray result ,qintptr descriptor)
{
    UTF8ToGB2312(result);
    privateHash->clients[descriptor]->write(result);
}

void TCPServer::FileTransSlot(QByteArray result, qintptr descriptor)
{
    QByteArray length ;
    QDataStream d(&length,QIODevice::ReadWrite);
    d<<result.size();
    privateHash->clients[descriptor]->write(QString("ok").toUtf8().data());//这里的顺序不能换，结合排版软件端
    privateHash->clients[descriptor]->write(length);
    privateHash->clients[descriptor]->write(result);
}

void TCPServer::DisconnectSlot(qintptr socketDescriptor)
{
    hisCount+=1;
    privateHash->clients.remove(socketDescriptor);
    return;
}

void TCPServer::StartPrintSlot(QString filePath)
{
    emit StartPrintSignal(filePath);
}

void TCPServer::ContinuePrintSlot()
{
    emit ContinuePrintSignal();
}

void TCPServer::ProjectionIMGSlot(QString &imgPath)
{
    emit ProjectionIMGSignal(imgPath);
}

void TCPServer::UTF8ToGB2312(QByteArray &code)
{
    QTextCodec* utf8Codec= QTextCodec::codecForName("utf-8");
    QTextCodec* gb2312Codec = QTextCodec::codecForName("gb2312");

    QString strUnicode= utf8Codec->toUnicode(code);
    QByteArray ByteGb2312= gb2312Codec ->fromUnicode(strUnicode);

    code= ByteGb2312;
}
