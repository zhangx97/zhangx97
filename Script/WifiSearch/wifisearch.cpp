#include "wifisearch.h"
#include "ui_wifisearch.h"
#include <stdlib.h>

WifiSearch::WifiSearch(QObject *parent): QAbstractListModel(parent)
{
    propertyName[NameRole] = "wifiName";
    propertyName[QualityRole] = "wifiQuality";
    SearchWifi();
    checkNet = new NetworkActiveCheck();
    wifiCheck = new QTimer();
}

WifiSearch::~WifiSearch()
{
    clear();
    delete checkNet;
    delete wifiCheck;
}

void WifiSearch::connectWifi()
{
    qDebug()<<"开始连接wifi："<<wifiName<<"密码："<<wifiPassword;
    QString name = wifiName;
    QString password = wifiPassword;
    QString cmd = QString("sudo wpa_cli -i wlan0 remove_network 0;"
                          "sudo wpa_cli -i wlan0 add_network;sudo wpa_cli -i wlan0 set_network 0 ssid '\"%1\"';"
                          "sudo wpa_cli -i wlan0 set_network 0 psk '\"%2\"';"
                          "sudo wpa_cli -i wlan0 enable_network 0;"
                          "wpa_cli -i wlan0 save_config;\n").arg(name,password);
    qDebug().noquote()<<"连接指令："<<cmd;    //如何执行一个列表的指令
    system(cmd.toUtf8());


    wifiCheck->start(15000);
    connect(wifiCheck,&QTimer::timeout,this,&WifiSearch::CheckWIFI);

}

void WifiSearch::CheckWIFI()
{
    if(checkNet->LocalNetTest())
    {
        emit netCheckResult(true);
        wifiCheck->stop();
    }
    else
    {
        emit netCheckResult(false);
        wifiCheck->stop();
    }
}

QString WifiSearch::GetName() const
{
    return wifiName;
}

void WifiSearch::SetName(QString name)
{
    wifiName = name;
}

bool WifiSearch::GetWifiEnable(){
    return ConfigAndState::enableWifi;
}

void WifiSearch::SetWifiEnable(bool enable){
    ConfigAndState::enableWifi = enable;
}

QString WifiSearch::GetPassword() const
{
    return wifiPassword;
}

void WifiSearch::SetPassword(QString password)
{
    wifiPassword = password;
}

QString WifiSearch::GetCurrentWifi(){
    // 执行命令并获取结果
    QProcess process;
    process.start("sudo wpa_cli -i wlan0 status");
    process.waitForFinished();
    QString currentWifiMessage = process.readAllStandardOutput();

//     QString currentWifiMessage;
     QStringList splitMessage;
     splitMessage = currentWifiMessage.split("\n");
     for (int i=0;i<splitMessage.count();i++) {
         QString findName = splitMessage[i];
         if(findName.contains("ssid"))
         {
             currentWifi = ChangeHexToName(findName.split("=")[1]);
         }
     }
    return currentWifi;
}

void WifiSearch::clear()
{
    int count = allWifi.count();
    if(count>0)
    {
        for(int i = 0;i<count;i++)
        {
            delete allWifi.at(i);
        }
        allWifi.clear();
    }
}

void WifiSearch::Refresh()
{
    beginResetModel();
    clear();
    SearchWifi();
    endResetModel();
}

int WifiSearch::rowCount(const QModelIndex &parent) const
{
    return allWifi.count();
}

QHash<int,QByteArray> WifiSearch::roleNames() const
{
    return propertyName;
}

QVariant WifiSearch::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if(row <0 || row > allWifi.size())
    {
        return QVariant();
    }
    wifiFind *f = allWifi[index.row()];
    return f->at(role - Qt::UserRole);
}

//搜索wifi并排序
void WifiSearch::SearchWifi()
{
    qDebug()<<"搜索附近的wifi";
    // 执行命令并获取结果
    QProcess process;
    process.start("sudo wpa_cli scan_result");
    process.waitForFinished();
    QString wifiContent = process.readAllStandardOutput();
//    qDebug()<<"获取到的wifi信息:"<<wifiContent;

//    QString wifiContent;    //读取wifi信息
    QStringList wifiContentList;    //分割wifi信息
    int tempWifiQuality;
    QString tempWifiName;
    wifiContentList = wifiContent.split("\n");
    wifiFind *findWifi;
    for(int i = 2;i<wifiContentList.count()-1;i++)
    {
//        qDebug()<<"进入搜索wifi的列表循环";
        findWifi = new wifiFind();
        QStringList wifiDetail;     //每条wifi的详细信息
        wifiDetail = wifiContentList[i].split("\t");
        int quality = wifiDetail[2].toInt();
        tempWifiQuality = 100 + wifiDetail[2].toInt();
        tempWifiName = ChangeHexToName(wifiDetail[4]);
        findWifi->append(tempWifiName);
        findWifi->append(QString::number(tempWifiQuality));
//            WifiMessage.insert(tempWifiName,wifiFlagAndSignal);
        allWifi.append(findWifi);
        findWifi = 0;
    }
    if(allWifi.count()>0)
    {
        wifiListQuickSort(allWifi,0,allWifi.count()-1);
    }
    else
    {
        findWifi = new wifiFind();
        findWifi->append("wifi模块正在初始化(一分钟)");
        findWifi->append(QString::number(0));
        allWifi.append(findWifi);
        findWifi = 0;
    }
}

QString WifiSearch::ChangeHexToName(QString wifiName){
    QString str1 = wifiName;
    if(str1.contains("\\x"))
    {
        int pos = 0;

        QRegExp catchHex("([\\\\x][A-Za-z0-9]{3})+");

        while ((pos = catchHex.indexIn(str1, pos)) != -1)
        {
            int length = catchHex.matchedLength();
            QByteArray hex = QByteArray::fromHex(catchHex.cap(0).toUtf8());
            QString name = QString::fromUtf8(hex);
            str1.replace(pos,length,name);
            pos = name.length();
        }
        return str1;
    }
    else
    {
        return str1;
    }
}

//快速排序，信号强的在前，弱的在后
void WifiSearch::wifiListQuickSort(QVector<wifiFind*> &list ,int begin ,int end)
{
    if(begin<end)
    {
        wifiFind *tempWifi = new wifiFind();
        tempWifi = list[begin];//保存基准元素
        int i = begin;
        int j = end;
        while(i<j)
        {
            //j值小于基准值时，左移一位
            while(i<j && (*list[j])[1]<(*tempWifi)[1])
            {
                j--;
            }
            //大于基准值的j，与i位置交换
            list[i] = list[j];
            //i值大于基准值时，右移一位
            while(i<j && (*list[i])[1]>=(*tempWifi)[1])
            {
                i++;
            }
            //i值小于基准值时，与j位置交换
            list[j] = list[i];
        }
        //将值填入中间区域
        list[i] = tempWifi;
        //对左右两侧的剩余区域进行排序
        wifiListQuickSort(list,begin,i-1);
        wifiListQuickSort(list,i+1,end);
    }
    else
    {
        return;
    }
}




