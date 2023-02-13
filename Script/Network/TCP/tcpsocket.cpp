#include "Script/Network/TCP/tcpsocket.h"
#include <QThread>
#include <QTextCodec>
#include <QDataStream>
#include <stdlib.h>

TcpSocket::TcpSocket(qintptr socketDescriptor,QObject *parent):QTcpSocket (parent),handle(socketDescriptor)
{
    this->setSocketDescriptor(handle);
    connect(this, &TcpSocket::readyRead, this, &TcpSocket::ReadDataSlot);
    connect(this, &TcpSocket::disconnected, this, &TcpSocket::DisconnectSlot);
//    connect(this, &TcpSocket::disconnected, this, &TcpSocket::deleteLater);
}

TcpSocket::~TcpSocket()
{
    deleteLater();
}


void TcpSocket::ReadDataSlot()
{
//    char headFlag[5];
    char byteShow[4];
    QString fileFlag = "ok";

    QByteArray buffer;
    QDataStream in(this);

//        qDebug()<<"块为0";
    if(bytesAvailable()<4)
    {
        return;
    }
    else
    {
        if(!fileMode)
        {
            buffer = readAll();
            QString str = byteToUTF8(buffer);
            if(str == "file:")
            {
                fileMode = true;
//                qDebug()<<"file指令"<<fileFlag.size()<<fileFlag.toUtf8().data();
                write(fileFlag.toUtf8().data());
            }
            else
            {
                qDebug()<<"收到的非文件指令："<<buffer;
                emit SendCMDSignal(buffer,this->socketDescriptor());
            }
        }
        else
        {

            if(!waitForByte)
            {
                in.readRawData(byteShow,4);
                QByteArray sizeByte(byteShow);
                blockSize = bytesToInt(sizeByte.data());
//                qDebug()<<"包的大小："<<QString::number(blockSize);
            }

            if(bytesAvailable()<blockSize)
            {
                waitForByte = true;
                return;
            }

            QByteArray packContent = readAll();
//                qDebug()<<"文件传输时接收到的包头："<<byteToUTF8(packContent);
            PackAnalyze(packContent);
//            qDebug()<<"一次包分析完毕";
            write(fileFlag.toUtf8().data());
            fileMode = false;
            waitForByte = false;
        }
    }

}

void TcpSocket::PackAnalyze(QByteArray pack)
{
    if(pack.contains("PackName"))
    {
        emit SendCMDSignal(pack,this->socketDescriptor());

    }
    else if(pack.contains("initPrint"))
    {
        qDebug()<<"初始化打印:"<<pack;
        emit SendCMDSignal(pack,this->socketDescriptor());
    }
    else if(pack.contains("name"))
    {
//        qDebug()<<"文件名："<<QString(pack).split("//")[1];
        reciveFile.fileName = QString(pack).split("//")[1].split(".")[0]+QString(".zip");
    }
    else if(pack.contains("date"))
    {
//        qDebug()<<"创建日期："<<QString(pack).split("//")[1];
        reciveFile.fileCreateDate = QString(pack).split("//")[1];
    }
    else if(pack.contains("size"))
    {
//        qDebug()<<"完整包尺寸："<<QString(pack).split("//")[1];
        reciveFile.packSize = QString(pack).split("//")[1].toInt();
    }
    else if(pack.contains("count"))
    {
//        qDebug()<<"完整包数量："<<QString(pack).split("//")[1];
        reciveFile.packCount = QString(pack).split("//")[1].toInt();
    }
    else if(pack.contains("lastSize"))
    {
//        qDebug()<<"最后一个包的大小："<<QString(pack).split("//")[1];
        reciveFile.lastPackSize = QString(pack).split("//")[1].toInt();
    }
    else if(pack.contains("who are you"))
    {
        QString type = QString::number(ConfigAndState::modelType);
        int putIn = type.size();
        for(int i =0 ;i<(7-putIn);i++)
        {
//            qDebug()<<"循环i值："<<QString::number(i);
            type.append("0");
        }
//        qDebug()<<"返回机型串"<<type;
        write(QString("ok").toUtf8().data());
        write(type.toUtf8().data());
    }
    else
    {
        filelist.append(pack);
        packCounts+=1;
        if(packCounts == 1)
        {
//            qDebug()<<"首包："<<filelist;
        }
//        qDebug()<<"存储次数："<<packCounts;
    }
    if(packCounts  == reciveFile.packCount+1)
    {
//        qDebug()<<filelist;
        reciveFile.save(filelist);
    }
}

int TcpSocket::bytesToInt(QByteArray bytes)
{
    int addr = bytes[0] & 0x000000FF;
    addr |= ((bytes[1] << 8) & 0x0000FF00);
    addr |= ((bytes[2] << 16) & 0x00FF0000);
    addr |= ((bytes[3] << 24) & 0xFF000000);
    return addr;
}

QString TcpSocket::byteToUTF8(QByteArray bytes)
{
    QTextCodec *tc = QTextCodec::codecForName("UTF8");
    QByteArray ba = bytes;
    QString str = tc->toUnicode(ba);
    return str;
}

void TcpSocket::DisconnectSlot()
{
    abort();
    close();
    this->deleteLater();
    emit DisconnectSingal(handle);
//    qDebug()<<"收到断开信号，标识符："<<handle;
}
