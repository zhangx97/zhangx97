#include "Script/StateMachine/config_and_state.h"
#include "Script/Print3DControler/xml_operation.h"

#include <QFileInfo>
#include <QMutex>
#include <QMutexLocker>
#include <QTextCodec>

//初始化静态变量------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------A
bool ConfigAndState::applyMask = true;                                 //应用mask
int ConfigAndState::ascendSpeed= 0;                                    //电机上升速度，单位：转/分钟
//--------------------------------------------------------------------------------------------------B
qint32 ConfigAndState::baudRate = 9600;                                //串口通信的波特率
QStringList ConfigAndState::bottomCuringTimes;                         //底片的固化时间
int ConfigAndState::bottomCount = 0;                                   //底片的层数
//--------------------------------------------------------------------------------------------------C
QStringList ConfigAndState::continueCommand;                           //
int ConfigAndState::crackDistance= 0;                                  //
int ConfigAndState::curBrightness = 0;                                 //当前亮度
int ConfigAndState::curCurrent = 0;                                    //当前电流
//int ConfigAndState::curHeatData = 0;                                   //
int ConfigAndState::curLEDTemp = 0;                                    //led温度
//int ConfigAndState::curResinTemp = 0;                                  //
//--------------------------------------------------------------------------------------------------D
int ConfigAndState::descendSpeed= 0;                                   //电机下降速度，单位：转/分钟
int ConfigAndState::device_power_pin= 16;                              //
QString ConfigAndState::deviceSerialName="";                           //设备串口
int ConfigAndState::device_temp_pin = 20;                              //
double ConfigAndState::downloadPercent = 0.0;                          //安装包的下载进度
//--------------------------------------------------------------------------------------------------E
bool ConfigAndState::enableWifi = true;                                //是否允许开启wifi
//--------------------------------------------------------------------------------------------------F
//QString ConfigAndState::file_path = "/home/pi/python_project";       //文件路径
int ConfigAndState::finishCount = 0;                                   //已打印完成的层数
bool ConfigAndState::firstReadFlag = false;                            //用来标记是否第一次读取数据
//--------------------------------------------------------------------------------------------------H
QString ConfigAndState::heater="0,0.0";                                //返回给排版软件打印所需的温度值以及当前的温度
QString ConfigAndState::heaterSwitch = "";                             //加热功能的开关
//--------------------------------------------------------------------------------------------------I
bool ConfigAndState::internetAlive = false;                            //互联网能否ping通
bool ConfigAndState::isInUpdateStatus = false;                         //是否处于下载更新包的状态
bool ConfigAndState::isLocker = false;                                 //大概是旧机器用来判断门有没有锁的，应该是用不到了，总之先留着
//bool ConfigAndState::isSleep = true;                                   //是否在待机状态
//--------------------------------------------------------------------------------------------------L
double ConfigAndState::layerThickness = 0;                             //切层厚度
QByteArray ConfigAndState::lightengineSN = "NULL";                     //光机序列号
int ConfigAndState::lightSensorType=0;                                 //光机类型
int ConfigAndState::lightValueOffset=0;                                //光机亮度偏差值
bool ConfigAndState::localNetAlive = false;                            //局域网能否ping通
//--------------------------------------------------------------------------------------------------M
QString ConfigAndState::mask_img = "";                                 //掩码图片路径
QString ConfigAndState::material = "NULL";                             //打印材料
double ConfigAndState::maxZ = 0;                                       //切层总高度
int ConfigAndState::modelCount = 0;                                    //不包括底片的模型层数
int ConfigAndState::modelPicHeight = 0;                                //模型切层图的高度
QString ConfigAndState::modelName = "NULL";                            //模型名
int ConfigAndState::modelType=0;                                       //打印机机型
int ConfigAndState::modelVolume;                                       //模型体积
int ConfigAndState::modelPicWidth = 0;                                 //模型切层图的宽度
int ConfigAndState::motorMoveTime = 3000;                              //电机移动花费时间,单位毫秒
double ConfigAndState::movementTime = 0;                               //移动时间（暂时没有计算，也没有地方用到），旧机器里有，所以留着
//--------------------------------------------------------------------------------------------------N
bool ConfigAndState::newFileRecive = false;                            //是否收到了新的slicepack
//--------------------------------------------------------------------------------------------------P
QStringList ConfigAndState::pauseCommand;                              //
int ConfigAndState::port= 0;                                           //端口号

QString ConfigAndState::printException = "NULL";                       //
//int ConfigAndState::PrintFileSize = 0;                                 //待打印文件的大小
int ConfigAndState::PrintInitState = 0;                                //在打印初始化阶段时，要打印的文件是否存在
                                                                           //0表示未进行初始化阶段
                                                                           //1表示初始化阶段无该文件
                                                                           //2表示初始化阶段有该文件
QString ConfigAndState::printID= "";                                   //打印机序列号
//QString ConfigAndState::PrintInitFileName = "";                        //要打印的文件名是什么，这个文件名包含后缀.zip
QString ConfigAndState::printStartTime = "2017-01-01 00:00:00";        //打印开始时间，从modelpack中读取
QString ConfigAndState::printState = "NULL";                           //打印机的状态，NULL时是空闲
int ConfigAndState::pluseCount = 130000;                               //平台从最顶部到达最底部经过的脉冲数
//--------------------------------------------------------------------------------------------------R
//int ConfigAndState::residueCount = 0;                                  //打印时剩余的层数
double ConfigAndState::resinCuringTime = 0;                            //模型每层的固化时间
double ConfigAndState::resinTemp = 0.0;                                //温度探头读到的温度
//--------------------------------------------------------------------------------------------------S
QString ConfigAndState::serialTimeOut = "";                            //串口超时时间
int ConfigAndState::sleepTime=0;                                       //多长时间后待机
QStringList ConfigAndState::specialExposeLayer;                        //特殊层
QStringList ConfigAndState::startCommand;                              //
QStringList ConfigAndState::stopCommand;                               //
QStringList ConfigAndState::supportParament;                           //支撑参数
//--------------------------------------------------------------------------------------------------T
int ConfigAndState::targetLight=0;                                     //光机目标亮度
double ConfigAndState::targetResinTemp = 0;                               //加热的目标温度
QString ConfigAndState::TcpHostIp= "";                                 //打印机ip
QString ConfigAndState::temp_path = "/sys/bus/w1/devices/";            //读取温度的路径
bool ConfigAndState::Test_GetPress = false;                            //循环测试压力传感器
int ConfigAndState::timeIntervalToCheckNet = 30000;                    //检查网络状态的时间间隔
//--------------------------------------------------------------------------------------------------V
QString ConfigAndState::version = "";                                  //版本
//TFSer = NULL;                                                        //串口通信用的，还没做到
//DevSer = NULL;                                                       //串口通信用的，还没做到
//nSleepTime = datetime.datetime.now();                                //用于计算休眠的，还没用到
//------------------------------------------------------------------------------------------
int ConfigAndState::slicepackMaxNum= 100;

//确定所有表示打印机状态的静态变量的值
void InitializeConfig()
{
   if(!ConfigAndState::firstReadFlag)
   {
        qDebug()<<"打印机参数初始化";
        //_________________realmaker.config xml配置文件中的参数___________________________________________________________
        XMLOperation *xmlConfig = new XMLOperation("../config/realmakerSetting.config");
        ConfigAndState::version = xmlConfig->ReadXMLText("主程序版本");
        if (xmlConfig->ReadXMLText("应用掩膜") == "true")
        {
           ConfigAndState::applyMask = true;
        }else
        {
           ConfigAndState::applyMask = false;
        }
        ConfigAndState::deviceSerialName=xmlConfig->ReadXMLText("串口名");
        ConfigAndState::serialTimeOut = xmlConfig->ReadXMLText("串口超时时间");
        ConfigAndState::bottomCuringTimes = xmlConfig->ReadXMLText("底片时间").split(",",QString::SkipEmptyParts);
        ConfigAndState::crackDistance = xmlConfig->ReadXMLText("防压裂距离").toInt();
        ConfigAndState::ascendSpeed = xmlConfig->ReadXMLText("上升速度").toInt();
        ConfigAndState::descendSpeed = xmlConfig->ReadXMLText("下降速度").toInt();
        ConfigAndState::TcpHostIp = ConfigAndState::GetIPAddress();
        xmlConfig->WriteXMLData("ip地址",ConfigAndState::TcpHostIp);

        ConfigAndState::port = xmlConfig->ReadXMLText("ip端口").toInt();
        ConfigAndState::targetLight = xmlConfig->ReadXMLText("目标亮度").toInt();

        QString readHeater=xmlConfig->ReadXMLText("温度");
        if(readHeater == "0")
        {
           ConfigAndState::heater = "0,0.0";
        }
        else
        {
           ConfigAndState::heater = QString("%1,0.0").arg(readHeater);
        }
        ConfigAndState::heaterSwitch = xmlConfig->ReadXMLText("加热开关");
        ConfigAndState::targetResinTemp = xmlConfig->ReadXMLText("目标温度").toDouble();
        ConfigAndState::sleepTime = xmlConfig->ReadXMLText("休眠时间").toInt();
        ConfigAndState::printID = xmlConfig->ReadXMLText("序列号");
        ConfigAndState::modelType = xmlConfig->ReadXMLText("机型").toInt();
        ConfigAndState::lightSensorType = xmlConfig->ReadXMLText("光机类型").toInt();
        ConfigAndState::lightValueOffset = xmlConfig->ReadXMLText("亮度误差").toInt();
        ConfigAndState::pluseCount = xmlConfig->ReadXMLText("测压脉冲数").toInt();
        ConfigAndState::slicepackMaxNum = xmlConfig->ReadXMLText("slicepack文件上限").toInt();
        if(xmlConfig->ReadXMLText("局域网联通") == "true")
        {
//           ConfigAndState::localNetAlive = true;
        }
        else
        {
//           ConfigAndState::localNetAlive =  false;
        }
        if(xmlConfig->ReadXMLText("互联网联通") == "true")
        {
//           ConfigAndState::internetAlive = true;
        }
        else
        {
//           ConfigAndState::internetAlive =  false;
        }
        ConfigAndState::timeIntervalToCheckNet = xmlConfig->ReadXMLText("网络检查时间").toInt();
        delete  xmlConfig;
        //print_cmd.cmd的参数---------------------------------------------------------------------------------------------------------------
        QFile commandToArduinoFile("../config/print_cmd.cmd");
        if(!commandToArduinoFile.open(QIODevice::ReadOnly))
        {
           std::cerr<<"Cannot open file for reading print_cmd.cmd"<<std::endl;
           qWarning()<<"无法打开print_cmd.cmd";
           return;
        }
        QTextStream commandStream(&commandToArduinoFile);
        ConfigAndState::startCommand = commandStream.readLine().split(" ",QString::SkipEmptyParts);
        ConfigAndState::startCommand.removeFirst();
        for(int i=0;i<ConfigAndState::startCommand.length();i++)
        {
           ConfigAndState::startCommand[i] = ConfigAndState::startCommand[i] + "\n";
        }

        ConfigAndState::pauseCommand = commandStream.readLine().split(" ",QString::SkipEmptyParts);
        ConfigAndState::pauseCommand.removeFirst();
        for(int i=0;i<ConfigAndState::pauseCommand.length();i++)
        {
           ConfigAndState::pauseCommand[i] = ConfigAndState::pauseCommand[i] + "\n";
        }

        ConfigAndState::continueCommand = commandStream.readLine().split(" ",QString::SkipEmptyParts);
        ConfigAndState::continueCommand.removeFirst();
        for(int i=0;i<ConfigAndState::continueCommand.length();i++)
        {
           ConfigAndState::continueCommand[i] = ConfigAndState::continueCommand[i] + "\n";
        }

        ConfigAndState::stopCommand = commandStream.readLine().split(" ",QString::SkipEmptyParts);
        ConfigAndState::stopCommand.removeFirst();
        for(int i=0;i<ConfigAndState::stopCommand.length();i++)
        {
           ConfigAndState::stopCommand[i] = ConfigAndState::stopCommand[i] + "\n";
        }
   }
}


//构造函数，调试的时候看看有没有成功，其它没有用
ConfigAndState::ConfigAndState()
{
    if(!ConfigAndState::firstReadFlag)
    {
        qInfo()<<"状态机-config初始化";
        firstReadFlag = true;
    }
}

void ConfigAndState::SetPrintState(QString value)
{
    QMutex mutex;
    QMutexLocker locker(&mutex);

    ConfigAndState::printState = value;
}

QString ConfigAndState::GetPrintState()
{
    QMutex mutex;
    QMutexLocker locker(&mutex);

    return ConfigAndState::printState;
}

void ConfigAndState::SetFinishCount(int value)
{
    QMutex mutex;
    QMutexLocker locker(&mutex);

    ConfigAndState::finishCount = value;
}

int ConfigAndState::GetFinishCount()
{
    QMutex mutex;
    QMutexLocker locker(&mutex);

    return ConfigAndState::finishCount;
}

QString ConfigAndState::GetIPAddress(){
    QList<QHostAddress> list =QNetworkInterface::allAddresses();
    foreach (QHostAddress address, list)
    {
       if(address.protocol() ==QAbstractSocket::IPv4Protocol)
       {
           if(address.toString() == "127.0.0.1")
           {
               continue;
           }
           return address.toString();
       }
    }
    return "";
}

bool ConfigAndState::GetSliceStatus()
{
    QMutex mutex;
    QMutexLocker locker(&mutex);
    return ConfigAndState::newFileRecive;
}

void ConfigAndState::SetSliceStatus(bool status)
{
    QMutex mutex;
    QMutexLocker locker(&mutex);
    ConfigAndState::newFileRecive = status;
}

bool ConfigAndState::CheckPackExist(QString path,int packSize)
{
    QFileInfo fileInfo(path);
    if(fileInfo.isFile())
    {
//        qDebug()<<"文件大小："<<packSize;
        if(packSize>0)
        {
            int size = fileInfo.size();
//            qDebug()<<"本地文件大小: "<<size;
//            qDebug()<<"外部文件大小："<<packSize;
            if(size == packSize)
            {
//                qDebug()<<"文件存在";
                return  true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return true;

        }
    }

    return false;
}

bool ConfigAndState::CheckPackExist(QString path, QString packName,int packSize)
{
    QFileInfo fileInfo(path+packName);
    if(fileInfo.isFile())
    {
//        qDebug()<<"文件大小："<<packSize;
        if(packSize>0)
        {
            int size = fileInfo.size();
//            qDebug()<<"本地文件大小: "<<size;
//            qDebug()<<"外部文件大小："<<packSize;
            if(size == packSize)
            {
//                qDebug()<<"文件存在";
                return  true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return true;

        }
    }

    return false;
}

QString ConfigAndState::CalculateResidueTime()
{
    int tempFinishCount = ConfigAndState::GetFinishCount();//已打印的层数
    int tempBottomCount = ConfigAndState::bottomCount;//底片的数量
    int tempModelCount = ConfigAndState::modelCount;//除去底片层数的模型层数
    double tempCuring = ConfigAndState::resinCuringTime;//普层固化时间
    double motorMoveTime = (double)ConfigAndState::motorMoveTime/1000;//把单位从毫秒转换为秒
    double seconds = 0;//打印完毕预计需要的时间
    if(tempFinishCount<tempBottomCount)//还停留在底片阶段
    {
        int remainBottomCount = tempBottomCount - tempFinishCount;
        for (int i=tempFinishCount;i<tempBottomCount;i++) {
            seconds += ConfigAndState::bottomCuringTimes[i].toInt();
        }
        seconds += motorMoveTime * remainBottomCount;
        seconds += tempModelCount*tempCuring;
        seconds += motorMoveTime * (tempModelCount-1);
    }
    else//已经开始普通层打印
    {
        int remainCount = (tempBottomCount+tempModelCount)-tempFinishCount;
        seconds += remainCount*tempCuring;
        seconds += (remainCount-1)*motorMoveTime;
    }
    int int_s = (int)seconds;
    int h = int_s/(60*60);
    int m =(int_s-(h*60*60))/60;
    int sec = (int_s-(h*60*60))-m*60;

    QString hour = QString::number(h);
    if(hour.length()==1) hour = "0"+hour;
    QString minute = QString::number(m);
    if(minute.length()==1) minute = "0"+minute;
    QString second = QString::number(sec);
    if(second.length()==1) second = "0"+second;

    QString remainTimeString = hour+":"+minute+":"+second;
    return remainTimeString;
}

void ConfigAndState::deleteExtraFile(QString filetype)
{
    QDir::setCurrent("/home/pi/python_project/core");
    QStringList CurrentAllNameFilters;
    CurrentAllNameFilters << filetype;
    QDir slicepackdir("../slicepack/");
    QStringList CurrentAllFiles = slicepackdir.entryList(CurrentAllNameFilters,QDir::Files|QDir::Readable,QDir::Time);
    int deletenum = CurrentAllFiles.count() - ConfigAndState::slicepackMaxNum;
    if (deletenum > 0)
    {
        for (int j = 0; j < deletenum; j++)
        {
            QString deleteSlicepackFile = QString("../slicepack/%1.zip").arg(CurrentAllFiles[CurrentAllFiles.count()-j-1].split(".")[0]);
            QString deleteUnzipSlicepackDir = QString("../unzipSlicepack/%1").arg(CurrentAllFiles[CurrentAllFiles.count()-j-1].split(".")[0]);
            system(QString("sudo rm %1 -rf").arg(deleteSlicepackFile).toUtf8());
            qDebug() << "删除slicepack目录下旧的超过最大文件限制的文件 :" << QString("sudo rm %1 -rf").arg(deleteSlicepackFile);
            system(QString("sudo rm %1 -rf").arg(deleteUnzipSlicepackDir).toUtf8());
            qDebug() << "删除slicepack目录下旧的超过最大文件限制的文件夹 :" << QString("sudo rm %1 -rf").arg(deleteUnzipSlicepackDir);
        }

    }
}

//int ConfigAndState::GetResidueCount()
//{
//    QMutex mutex;
//    QMutexLocker locker(&mutex);
//    return ConfigAndState::residueCount;
//}

//void ConfigAndState::SetResidueCount(int residue)
//{
//    QMutex mutex;
//    QMutexLocker locker(&mutex);
//    ConfigAndState::residueCount = residue;
//}
