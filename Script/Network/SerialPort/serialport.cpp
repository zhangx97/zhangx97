#include "Script/Network/SerialPort/serialport.h"

SerialPort::SerialPort():serialPort(new QSerialPort)
{
    serialPort->setBaudRate(ConfigAndState::baudRate,QSerialPort::AllDirections);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);

    XMLOperation SerialInfo("../config/realmakerSetting.config");
    ArduinoPortName = SerialInfo.ReadXMLText("串口名");
//    qDebug()<<ArduinoPortName;
    OpenPort(ArduinoPortName);
    if(ReceiveInfo())
    {
        serialIsOpen = true;
    }
}

bool SerialPort::SerialPortInit()//初始化串口
{
    QByteArray info;
    while(serialPort->waitForReadyRead(1000))
    {
        if(serialPort->canReadLine())
        {
            QByteArray tempInfo = serialPort->readLine();
            if(tempInfo != "")
            {
                info += tempInfo;
            }
            else
            {
                continue;
            }

        }
    }
    if(info.indexOf("Start") != -1)
    {
//        qDebug()<<"arduino初始化完成";
        return true;
    }
}


SerialPort::~SerialPort()
{
    if(serialPort->isOpen())
    {
        serialPort->close();
        serialIsOpen = false;
    }
    delete serialPort;
}

bool SerialPort::OpenPort(QString portname)//打开串口
{
    if(serialPort->isOpen())
        {
            serialPort->clear();
            serialPort->close();
        }

    serialPort->setPortName(portname);

    if(!serialPort->open(QIODevice::ReadWrite))
    {
        qDebug()<<"打开失败";
        return false;
    }
    qDebug()<<QString("串口 %1 打开成功").arg(portname);

    return true;
}

bool SerialPort::ReceiveInfo(QByteArray message)//接收和判断串口信息
{
    QByteArray info;
    while(serialPort->waitForReadyRead(1000))
    {
        if(serialPort->canReadLine())
        {
            QByteArray tempInfo = serialPort->readLine();
            if(tempInfo != "")
            {
                info += tempInfo;
            }
            else
            {
                continue;
            }

        }
    }

    qDebug()<<"收到串口信息："<<info;

    if(info.indexOf("ER") != -1)
    {
        qWarning()<<QString("收到串口发来的错误信息：%1").arg(info.data());
        return false;
    }

    if(info.indexOf(message.trimmed()) != -1)   //接收到的信息与发送的指令相等
    {
//        qDebug()<<"接收到的信息包含发送指令";
        if(info.indexOf("OK")!=-1)
        {
            qDebug()<<QString("串口指令 %1 被顺利执行").arg(message.trimmed().data());
            return true;
        }
        else
        {
            while(serialPort->waitForReadyRead(-1))
            {
                if(serialPort->canReadLine())
                {
                    QByteArray infoOK = serialPort->readLine();
//                    qDebug()<<QString("读取到串口标记值：%1").arg(infoOK.data());
                    if(infoOK.indexOf("Press") != -1)
                    {
                        qDebug()<<"受到异常压力，请检查树脂槽内或平台上是否有异物，而后重新开始打印";

                        //循环压力测试时使用的标记
                        ConfigAndState::Test_GetPress = true;
                        return false;
                    }
                    else if(infoOK.indexOf("pluse") != -1)
                    {
                        if(message.trimmed() == "G28")
                        {
//                            qDebug()<<infoOK.data();
                            QString str = infoOK.data();
                            //写入xml文件
                            XMLOperation PluseCount("../config/realmakerSetting.config");
//                            PluseCount.ReadXML();
                            ConfigAndState::pluseCount = str.split(":")[1].toInt();
                            PluseCount.WriteXMLData("测压脉冲数",QString::number(ConfigAndState::pluseCount));
                            qDebug()<<"测压脉冲数 "<<QString::number(ConfigAndState::pluseCount);
                            return true;
                        }
                    }
                    else if(infoOK.indexOf("ER") != -1)
                    {
                        qDebug()<<"错误信息："<<info.data();
                        return false;
                    }
                    else if(infoOK == "")
                    {
                        continue;
                    }
                    else if(infoOK.indexOf("OK") != -1)
                    {
                        qDebug()<<QString("串口指令 %1 被顺利执行").arg(message.trimmed().data());
                        return true;
                    }
                    else
                    {
                        qDebug()<<QString("收到未知的串口信息：%1").arg(info.data());
                        return false;
                    }
                }
            }
        }
    }
    else    //接收到的信息与发送的指令不同
    {
        if(info.indexOf("OK") != -1)
        {
             qDebug()<<QString("串口指令 %1 被顺利执行").arg(message.trimmed().data());
             return true;
        }
    }
    qWarning()<<"预料之外的情况:"<<info;
    return false;
}

bool SerialPort::SendPort(QByteArray message)//发送串口信息
{
    if(serialIsOpen)
    {
        int successSendFlag;
        if(message.trimmed() == "G28")
        {
//            qDebug()<<"发送包含校准参数的指令";
            QString str = QString("PRESS_INIT %1\n").arg(ConfigAndState::pluseCount);
            qDebug()<<QString("发送消息:")<<str.toUtf8();
            serialPort->write(str.toUtf8().data());
            bool adjustReady = ReceiveInfo(str.toUtf8());
            if(adjustReady)
            {
                successSendFlag = serialPort->write(message.data());
                qDebug()<<QString("发送消息:%1").arg(QString(message));
            }
        }
        else
        {
            successSendFlag = serialPort->write(message.data());
            qDebug()<<QString("发送消息:%1").arg(QString(message));
        }


        if(ReceiveInfo(message) && (successSendFlag != -1))
        {
            return true;
        }
        return false;
    }
    else
    {
        qDebug()<<"串口未打开";
    }
    return false;

}
