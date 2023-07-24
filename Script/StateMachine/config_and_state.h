#ifndef GETCONFIG_H
#define GETCONFIG_H
#include <QObject>
#include <QString>
#include <QMap>
#include <QFile>
#include <QTextStream>
#include <iostream>
#include <QDebug>
#include <QtNetwork>
#include <QUdpSocket>
#include <QMutex>


void InitializeConfig();
//这个类用来获取系统需要的配置，系统的状态等一系列信息
class ConfigAndState:public QObject
{
    Q_OBJECT
public:

    ConfigAndState();
    void TriggerStartMessage();

//    static enum PrintState
//    {
//        null,
//        start,
//        pause,
//        continuePrint,
//        stop,
//        heating
//    };

    static void SetPrintState(QString);        //设置打印状态，避免多线程死锁
    static QString GetPrintState();
    static void SetFinishCount(int);
    static int GetFinishCount();
//    static void SetIfNeedUpdate(bool);
//    static bool GetIfNeedUpdate();
    static QString GetIPAddress();
    static bool GetSliceStatus();
    static void SetSliceStatus(bool);
    static bool CheckPackExist(QString path, QString packName,int packsize = 0);
    static bool CheckPackExist(QString path,int packsize = 0);
    static QString CalculateResidueTime();  //返回毫秒
//    static int GetResidueCount();
//    static void SetResidueCount(int);

    static bool firstReadFlag;//用来标记是否第一次读取数
    //配置文件中的内容
    static bool applyMask;//是否应用掩膜
    static QString serialTimeOut;//串口超时时间
    static qint32 baudRate;//串口波特率
    static int crackDistance;//防压裂距离
    static int ascendSpeed;//上升速度
    static int descendSpeed;//下降速度
    static QString TcpHostIp;//ip地址
    static int port;//ip端口
    static int targetLight;//目标亮度
    static QString heater;//返回给排版软件打印所需的温度值以及当前的温度
    static double resinTemp;//树脂槽的温度
    static double targetResinTemp;//目标温度
    static QString heaterSwitch;//加热开关
    static int sleepTime;//休眠时间
    static QString printID;//打印机序列号
//    static bool lookState;//门的加锁状态（大概）应该是用不到的----------------------
    static int modelType;//机型
    static QString deviceSerialName;//设备串口名
    static int lightSensorType;//光机类型，0普通，1大功率,2LCD
    static int lightValueOffset;//光机亮度允许的偏差值
    static QStringList startCommand;//开始打印时需要往arduino发送的命令
    static QStringList pauseCommand;//停止打印时需要往arduino发送的命令
    static QStringList continueCommand;//继续打印时需要往arduino发送的命令
    static QStringList stopCommand;//停止打印时需要往arduino发送的命令
    static int pluseCount;//平台从最顶部到达最底部经过的脉冲数
    static int device_power_pin;//打印机电源按钮的针脚--------------
    static int device_temp_pin;//设备温度传感器的针脚---------------
//    static int led_pin;//led针脚？不知道干嘛用的----------------------
    static QString version;//主程序版本
    static QString temp_path;//温度探头的路径
    static QString printException;//打印时遇到的错误信息
    static QByteArray lightengineSN;//光机序列号
    static int curBrightness;//当前亮度
    static int curCurrent;//当前电流
    static int curLEDTemp;//当前光机温度
//    static int tempDeviation;//温度偏差

//    static int curHeatData;//当前加热温度，大概是用不到-----------
    static bool isLocker;//为了兼容排版软件设置的，不用管
    static QString mask_img;//掩码图片路径
    static int modelCount;//模型层数---------------
    static int modelPicWidth;//投影图片宽
    static int modelPicHeight;//投影图片高
    static double layerThickness;//每层厚度
    static double maxZ;//切层总高度
    static int bottomCount;//底片层数
    static QStringList bottomCuringTimes;//底片固化时间
    static double resinCuringTime;//每层固化时间
    static QString modelName;//模型名
    static QString printStartTime;//为了兼容排版软件设置的，打印开始时间
    static QString material;//打印用的材料
    static int modelVolume;//模型的体积
    static QStringList supportParament;//支撑参数
    static QStringList specialExposeLayer;//特殊层的范围
    static int motorMoveTime;//电机移动花费时间,单位毫秒
    static double movementTime;//为了兼容排版软件设置的，移动速度
    static int timeIntervalToCheckNet;//隔一段时间测试网络联通情况
    static bool localNetAlive;//局域网能否ping通
    static bool internetAlive;//互联网能否ping通
    static bool enableWifi;//是否允许开启wifi
    //测试时用的变量
    static bool Test_GetPress;
    //记录打印初始化时的信息
    static int PrintInitState;//在打印初始化阶段时，要打印的文件是否存在,
                              //0表示未进行初始化阶段，该参数是为了与旧版软件兼容,指令发来时，如果是0，就按旧版的打印过程执行
                              //1表示初始化阶段要打印的文件不存在，
                              //2表示初始化阶段要打印的文件存在
    static double downloadPercent;//更新包下载的进度
    static bool isInUpdateStatus;//是否处于下载更新包的状态
    static int slicepackMaxNum;//slicepack文件上限

    static void deleteExtraFile(QString filetype);
signals:
    
private:
    QString messageNeedToSend;
    QMutex mutex;

    static QString printState;//当前打印状态.NULL是空闲，heat是加热中，start是正在打印，pause是暂停，stop是正在停止
    static int finishCount;//已打印完的层数
//    static int residueCount;//剩余的层数:（底片层数+模型层数）-已打印层数
    static bool newFileRecive;
    QString FormatTime(int ms);

};

#endif // READCONFIG_H
