#include "programcore.h"
#include "Script/lightEngine/lcdlight.h"

ProgramCore::ProgramCore(/*QObject *objectItem,*/QObject *parent)
    :QObject (parent)
{

//    initStep();

    //发送广播线程
    msg = "realmaker "+ ConfigAndState::printID;
    sendUDPBoardcast = new UDPNetwork;
    UDPSendThread = new QThread();
//    sendUDPBoardcast->moveToThread(UDPSendThread);
//    UDPSendThread->start();
//    udpSendTimer = new QTimer();
//    udpSendTimer->start(1000);
//    connect(udpSendTimer,&QTimer::timeout,this,&ProgramCore::UDPStart);
//    connect(this,&ProgramCore::UDPStartSignal,sendUDPBoardcast,&UDPNetwork::SendUDPBoardcast);

    //光机初始化
    lightInit = new CyUSBSerialLib(ConfigAndState::lightSensorType);
    bool states = lightInit->DeviceInit();
    if(!states)
    {
        qDebug()<<"光机初始化失败";
    }

    //LCD屏幕调光测试
//    QTimer LCDTest;
//    LCDTest.start(500);
//    connect(&LCDTest,&QTimer::timeout,this,&ProgramCore::LCDScreenGradientTest);


    ControlPrint = new PrintControl();
    PrintThread = new QThread();
    ControlPrint->moveToThread(PrintThread);
    PrintThread->start();

    connect(ControlPrint,&PrintControl::photoChange,this,&ProgramCore::changePrintImage);
    connect(ControlPrint,&PrintControl::StopPrint,this,&ProgramCore::stopPrintSlot);


    //开始tcp通信线程
    TCPServerInPrint = new TCPServer();
    TCPServerInPrint->listen(QHostAddress::Any,ConfigAndState::port);
    //打印控制
    connect(TCPServerInPrint,&TCPServer::StartPrintSignal,ControlPrint,&PrintControl::DealStartPrint);            //收到开始打印信号
    connect(TCPServerInPrint,&TCPServer::ContinuePrintSignal,ControlPrint,&PrintControl::ControlContinue);      //收到继续打印信号
    connect(TCPServerInPrint,&TCPServer::ProjectionIMGSignal,ControlPrint,&PrintControl::SetPhotoPath);    //投影
    //QML打印控制
    connect(this,&ProgramCore::QMLStartPrintSignal,ControlPrint,&PrintControl::DealStartPrint);
    connect(ControlPrint,&PrintControl::UnzipFinish,this,&ProgramCore::QMLUnzipFinishSlot);

    //网络连接测试
    NetCheck = new NetworkActiveCheck();
    NetCheck->NetworkTest();

    //加热控制
    HeatControl = new HeaterControl();
}

ProgramCore::~ProgramCore(){
    delete TCPServerInPrint;
    delete sendUDPBoardcast;
    delete UDPSendThread;
    delete udpSendTimer;
    delete serialTest;
    delete NetCheck;
    delete ControlPrint;
    delete HeatControl;
}

void ProgramCore::LCDScreenGradientTest()
{
    if(ConfigAndState::lightSensorType == 2)
    {
        bool forward = true;
        if(gradientTest<100 && gradientTest>0)
        {
            if(forward)
            {
                gradientTest+=5;
            }
            else
            {
                gradientTest-=5;
            }

        }
        else if(gradientTest>100 || gradientTest<0)
        {
            forward = !forward;
        }
        LCDLight lighttest;
        lighttest.open(gradientTest);
    }
}

void ProgramCore::UDPStart()
{
    emit UDPStartSignal(ConfigAndState::port,msg);
}

void ProgramCore::initStep()
{
    serialTest = new SerialPort();
    serialTest->SendPort("M18\n");
    delete serialTest;
}

void ProgramCore::PressTest()
{
    //循环压力测试
    qDebug()<<"开始串口测试";

    int testFlag = 1;
    while(true)
    {
        serialTest = new SerialPort();
        if(testFlag ==1 || ConfigAndState::Test_GetPress)
        {
            bool pressFlag = serialTest->SendPort("G28\n");

            if(!pressFlag)
            {
                serialTest->ReceiveInfo();
                qDebug()<<"get press,will start next test";
                testFlag++;
                qDebug()<<"Flag Test Count:"<<testFlag;
            }else
            {
                bool up = serialTest->SendPort("M18\n");
                if(!up){while(!serialTest->ReceiveInfo()){}}
            }
        }
        delete serialTest;
    }

}

void ProgramCore::changePrintImage(const QString &imagePath)
{
    emit PhotoChanged(imagePath);
}

void ProgramCore::QMLStartPrintSlot(QString slicepackPath)
{
    emit QMLStartPrintSignal(slicepackPath);
}

void ProgramCore::QMLUnzipFinishSlot()
{
    emit QMLUnzipFinish();
}

void ProgramCore::HeatSwitchChange(bool flag)
{
    if(flag)
    {
        HeatControl->OpenHeater();
    }
    else
    {
        HeatControl->StopHeater();
    }
}

void ProgramCore::stopPrintSlot()
{
    emit StopPrint();
}
