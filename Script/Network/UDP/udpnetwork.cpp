#include "Script/Network/UDP/udpnetwork.h"
#include "Script/StateMachine/config_and_state.h"

//#include <QHostInfo>
#include <QtNetwork>
#include <QUdpSocket>
#include <QTimer>

UDPNetwork::UDPNetwork(QObject *parent) : QObject(parent)
{

}



void UDPNetwork::SendUDPBoardcast(int toPort,QString msg)
{
    udpSocket = new QUdpSocket();
    int length=0;
    if(msg=="")
    {
       return;
    }
    if((length=udpSocket->writeDatagram(msg.toUtf8(),
    msg.length(),QHostAddress::Broadcast,toPort))!=msg.length())
    {
        return;
    }
    udpSocket->deleteLater();
    //qDebug()<<"start UDPBoardCast";

    QList<QHostAddress> list =QNetworkInterface::allAddresses();
    foreach (QHostAddress address, list)
    {
       if(address.protocol() ==QAbstractSocket::IPv4Protocol)
       {
           if(address.toString() == "127.0.0.1")
           {
               continue;
           }
           //我们使用IPv4地址
           ConfigAndState::TcpHostIp = address.toString();
           break;
       }
    }
}


