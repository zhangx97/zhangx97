#ifndef PROGRAMCORE_H
#define PROGRAMCORE_H

#include <QQmlApplicationEngine>

#include "Script/StateMachine/config_and_state.h"
#include "Script/Network/UDP/udpnetwork.h"
#include "Script/lightEngine/cyusbseriallib.h"
#include "Script/Network/SerialPort/serialport.h"
#include "Script/Network/TCP/tcpserver.h"
#include "Script/Network/NetworkActiveCheck/network_active_check.h"
#include "Script/Print3DControler/print_control.h"
#include "Script/HeaterControl/heatercontrol.h"


class ProgramCore:public QObject
{
    Q_OBJECT
public:
    ProgramCore(/*QObject *root,*/QObject *parent = 0);
    ~ProgramCore();

    //udp传输
    UDPNetwork *sendUDPBoardcast;
    QThread *UDPSendThread;
    QTimer *udpSendTimer;
    void UDPStart();

    //tcp传输
    TCPServer *TCPServerInPrint;
//    TCPServer TCPServerInPrint;

    //光机
    CyUSBSerialLib *lightInit;

    //循环压力测试
    SerialPort *serialTest;
    void PressTest();

    //网络连接测试
    NetworkActiveCheck *NetCheck;

    //打印控制
    PrintControl *ControlPrint;
    QThread *PrintThread;
    void stopPrintSlot();
    void printCompleteSlot();

    void changePrintImage(QString imagePath);
    //LCD屏幕渐变测试
    void LCDScreenGradientTest();
    int gradientTest = 0;
    //QML槽函数
    void QMLStartPrintSlot(QString slicepackPath);
    void QMLUnzipFinishSlot();

    //加热
    HeaterControl *HeatControl;
    void HeatSwitchChange(bool flag);

signals:
    void UDPStartSignal(int,QString);
    void QMLStartPrintSignal(QString slicepackPath);
    void QMLUnzipFinish();
    void PhotoChanged(QString path);
    void StopPrint();
    void PrintComplete();

private:
    QString msg;
//    QObject *m_engine;
    void initStep();
};

#endif // PROGRAMCORE_H
