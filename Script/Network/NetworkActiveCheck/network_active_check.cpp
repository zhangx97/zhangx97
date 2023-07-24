#include "Script/Network/NetworkActiveCheck/network_active_check.h"

NetworkActiveCheck::NetworkActiveCheck()
{
//    QTimer *timer = new QTimer;
//    timer->start(ConfigAndState::timeIntervalToCheckNet);
//    connect(timer,&QTimer::timeout,this,&NetworkActiveCheck::CheckNetworkOnline);
}
NetworkActiveCheck::~NetworkActiveCheck()
{

}

void NetworkActiveCheck::CheckNetworkOnline()
{
    QList<QHostAddress> list =QNetworkInterface::allAddresses();
    foreach (QHostAddress address, list)
    {
       if(address.protocol() == QAbstractSocket::IPv4Protocol)
       {
           if(address.toString() == "127.0.0.1")
           {
               continue;
           }
           //当前的IPv4地址
           currentIP = address.toString();      //实际应用时该ip从ConfigAndState获取
           qDebug()<<"当前ip"<<currentIP;
           break;
       }
    }

    NetworkTest();

    //-------------------------只在测试时使用-------------------------
    if(!localNetAlive)
    {
        ChangeIPAndRoute();
    }
    //--------------------------------------------------------------

}

int NetworkActiveCheck::NetworkTest()
{
    bool InternetTestReturn, LocalNetTestReturn;
    InternetTestReturn = InternetTest();
    LocalNetTestReturn = LocalNetTest();
    if (InternetTestReturn || LocalNetTestReturn)
    {
        return 2;
    }else if (!InternetTestReturn || !LocalNetTestReturn) {
        return 0;
    }else {
        return 1;
    }
}

bool NetworkActiveCheck::LocalNetTest()
{
    if(ConfigAndState::GetIPAddress() != ""){
        localNetAlive = true;
        ConfigAndState::localNetAlive = true;
        return true;
    }else{
        localNetAlive = false;
        ConfigAndState::localNetAlive = false;
        return false;
    }
}

bool NetworkActiveCheck::InternetTest()
{
    QString pingResult;
    QStringList pingResultList;
    qDebug()<<"开始互联网联通性检测";
    QProcess process;
    process.start("ping -c 3 -w 1 114.114.114.114");
    process.waitForFinished();
    pingResult = process.readAllStandardOutput();

    pingResultList = pingResult.split("\n");
    for(int i=0;i<pingResultList.count();i++)
    {
        if(pingResultList.at(i).contains("statistics"))
        {
            QStringList lossRateList = pingResultList.at(i+1).split(" ",QString::SkipEmptyParts);
            for(int j = 0;j<lossRateList.count();j++)
            {
                if(lossRateList.at(j).contains("packet"))
                {
                    if(lossRateList.at(j-1).contains("%"))
                    {
                        int lossRate = lossRateList.at(j-1).split("%")[0].toInt();
                        qDebug()<<"丢包率："<<lossRate;
                        if(lossRate==100)
                        {
                            qDebug()<<"与互联网的连接已断开";
                            internetAlive = false;
                            ConfigAndState::internetAlive = false;
                            return false;
                        }
                        else
                        {
                            internetAlive = true;
                            ConfigAndState::internetAlive = true;
                            qDebug()<<"互联网连接正常";
                            return true;
                        }
                    }
                }
            }
        }
    }
}

void NetworkActiveCheck::ChangeIPAndRoute()     //还需要改变网关那块
{
    GetConnectState();
    GetRoute();
    //设置需要更改的ip
    QStringList ipPointList = currentIP.split(".");
    int lastOfIP = ipPointList[3].toInt();
    if(lastOfIP<254 && lastOfIP>3 && addCutFlag)
    {
        lastOfIP++;
    }
    else if(lastOfIP<254 && lastOfIP>3 && !addCutFlag)
    {
        lastOfIP--;
    }
    else if(lastOfIP == 254)
    {
        addCutFlag = false;
        lastOfIP--;
    }
    else if (lastOfIP == 3)
    {
        addCutFlag = true;
        lastOfIP++;
    }

    ipPointList.removeLast();
    QString newIP = ipPointList.join(".")+"."+QString::number(lastOfIP);

    //读取树莓派网络配置文件
    QString fileText;
    QFile readFile("/etc/dhcpcd.conf");
    if(!readFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"文件打开失败：可能是权限不足";
        return;
    }
    else
    {
        QTextStream ReadStream(&readFile);
        fileText = ReadStream.readAll();
    }
    readFile.close();

    //修改配置文件并写入
    QStringList fileList;
    QFile writeFile("/etc/dhcpcd.conf");
    if(!writeFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug()<<"文件打开失败：可能是权限不足";
        return;
    }
    else
    {
        QTextStream writeStream(&writeFile);
        fileList = fileText.split("\n");
        for(int i =0;i<fileList.count();i++)
        {
            if(i == fileList.count()-1)
            {
                writeStream<<fileList.at(i);    //最后一行不添加换行符
            }
            else
            {
                if(fileList.at(i).contains(connectState))
                {
                    if(fileList.at(i-1).contains("Example static IP configuration"))
                    {
                        QString txtHead = fileList.at(i);
                        QString headChange = QString("interface %1").arg(connectState+"0");
                        txtHead.replace(0,txtHead.length(),headChange);
                        writeStream<<txtHead<<"\n";

                        for(int j=1;j<5;j++)
                        {
                            if(fileList.at(j+i).contains("static ip_address="))
                            {
                                QString tempStr=fileList.at(i+j);
                                QString IPChangeTo = QString("static ip_address=%1").arg(newIP);
                                tempStr.replace(0,tempStr.length(),IPChangeTo);
                                writeStream<<tempStr<<"\n";
                            }
                            else if(fileList.at(j+i).contains("static ip6_address"))
                            {
                                writeStream<<fileList.at(i+j)<<"\n";
                            }
                            else if(fileList.at(j+i).contains("static routers="))
                            {
                                QString tempStr=fileList.at(i+j);
                                QString route = QString("static routers=%1").arg(routeIP);
                                tempStr.replace(0,tempStr.length(),route);
                                writeStream<<tempStr<<"\n";
                            }
                            else if(fileList.at(j+i).contains("domain_name_servers"))
                            {
                                writeStream<<fileList.at(i+j)<<"\n";
                            }
                        }
                        i+=4;
                    }
                    else
                    {
                        writeStream<<fileList.at(i)+"\n";   //无关的行重新写入并添加换行符
                    }
                }
                else
                {
                    writeStream<<fileList.at(i)+"\n";   //无关的行重新写入并添加换行符
                }
            }
        }
        system("sudo ifconfig eth0 down");
        system("sudo ifconfig wlan0 down");
        system("sudo ifconfig eth0 up");
        system("sudo ifconfig wlan0 up");
    }
    writeFile.close();
}
//判断连接状态
void NetworkActiveCheck::GetConnectState()
{
    QString ifconfig;
    QStringList ifconfigList;

//    QDir tempRouteDir("../config/netAddress.txt");
//    QString filePath = tempRouteDir.absolutePath();
//    QString cmd = QString("sudo ifconfig>%1").arg(filePath);
//    system(cmd.toUtf8().data());

    QProcess process;
    process.start("sudo ifconfig");
    process.waitForFinished();
    ifconfig = process.readAllStandardOutput();

//    QFile Readfile("../config/netAddress.txt");
//    if(!Readfile.open(QIODevice::ReadOnly | QIODevice::Text))
//    {
//        qDebug()<<"读取ifconfig文件失败";
//        return;
//    }
//    else
//    {
//        qDebug()<<"分装ifconfig数据";
//        QTextStream ReadFileText(&Readfile);
//        ifconfig = ReadFileText.readAll();
        ifconfigList = ifconfig.split("\n");
        qDebug()<<"分析ifconfig";
        for(int i = 0;i<ifconfigList.count();i++)
        {
            //判断是否是有线连接
            if(ifconfigList.at(i).contains("eth0"))
            {
                qDebug()<<"分析"<<ifconfigList.at(i+1);
                QString tempGetIpv4 = ifconfigList.at(i+1);
                if(tempGetIpv4.contains("inet"))
                {
                    qDebug()<<tempGetIpv4.split(" ",QString::SkipEmptyParts)[0];
                    QString tempGetIpv4Address = tempGetIpv4.split(" ",QString::SkipEmptyParts)[1];
                    if(tempGetIpv4Address == currentIP)
                    {
                        connectState = "eth";
                        return;
                    }
                }
            }
            //判断是否是无线连接
            if(ifconfigList.at(i).contains("wlan0"))
            {
                QString tempGetIpv4 = ifconfigList.at(i+1);
                if(tempGetIpv4.contains("inet"))
                {
                    QString tempGetIpv4Address = tempGetIpv4.split(" ")[1];
                    if(tempGetIpv4Address == currentIP)
                    {
                        connectState = "wlan";
                        return;
                    }
                }
            }
        }
//    }
}

void NetworkActiveCheck::GetRoute()
{
    QDir tempRouteDir("../config/route.txt");
    QString filePath = tempRouteDir.absolutePath();
    QString cmd = QString("sudo ip route>%1").arg(filePath);
    system(cmd.toUtf8().data());
    QString route;
    QStringList routeList;

    QFile readFile(filePath);
    if(!readFile.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        qDebug()<<"读取route文件失败";
    }
    else
    {
        QTextStream fileStream(&readFile);
        route = fileStream.readAll();
        routeList = route.split("\n");
        for(int i = 0;i<routeList.count();i++)
        {
            if(routeList.at(i).contains(connectState) && routeList.at(i).contains("default"))
            {
                routeIP = routeList.at(i).split(" ")[2];
                qDebug()<<"routeIP:"<<routeIP;
            }
        }
    }

}
