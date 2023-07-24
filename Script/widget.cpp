#include "Script/widget.h"
#include "Script/Print3DControler/unzip.h"
#include "Script/Print3DControler/xml_operation.h"
#include "Script/lightEngine/light_engine.h"
#include "Script/lightEngine/cyusbseriallib.h"
#include "Script/Network/SerialPort/serialport.h"


#include "ui_widget.h"

#include <QThread>
#include <QMutexLocker>


Widget::Widget(QWidget *parent) :/*QOpenGLWidget (parent),*/
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    move(1920,0);
    setWindowFlags(Qt::FramelessWindowHint);

    //信息读取
    port = ConfigAndState::port;
    id = ConfigAndState::printID;

    //发送广播线程
    msg = "realmaker "+ id;
    sendUDPBoardcast = new UDPNetwork;
    UDPSendThread = new QThread();
    sendUDPBoardcast->moveToThread(UDPSendThread);
    UDPSendThread->start();

    udpSendTimer = new QTimer();
    udpSendTimer->start(1000);

//    connect(udpSendTimer,&QTimer::timeout,this,&Widget::UDPStart);
//    connect(this,&Widget::UDPStartSignal,sendUDPBoardcast,&UDPNetwork::SendUDPBoardcast);

    //光机初始化
//    CyUSBSerialLib lightInit;
//    //lightInit.getDeviceNumber();
//    bool states = lightInit.deviceInit();

//    //QThread::sleep(1);
//    lightInit.setLedCurrent(100);


//    if(!states)
//    {
//        qDebug()<<"光机初始化失败";
//    }


    //电机运转与串口通信测试
//    qDebug()<<"开始串口测试";
//    int testFlag = 1;
//    while(true)
//    {
//        if(testFlag ==1 || ConfigAndState::Test_GetPress)
//        {
//            SerialPort serialTest;
//            bool pressFlag = serialTest.SendPort("G28\n");

//            if(!pressFlag)
//            {
//                serialTest.ReceiveInfo();
//                qDebug()<<"get press,will start next test";
//                testFlag++;
//                qDebug()<<"Flag Test Count:"<<testFlag;
//            }else
//            {
//                bool up = serialTest.SendPort("M18\n");
//                if(!up){while(!serialTest.ReceiveInfo()){}}
//            }


//        }
//    }



    //开始tcp通信线程
    TCPServerInPrint = new TCPServer();

    qDebug()<<"主线程ID:"<<QThread::currentThreadId()<<"\n";

    //打印控制
    connect(TCPServerInPrint,&TCPServer::StartPrintSignal,this,&Widget::PrintStartSlot);            //收到开始打印信号
    connect(TCPServerInPrint,&TCPServer::ContinuePrintSignal,this,&Widget::PrintContinueSlot);      //收到继续打印信号
    connect(TCPServerInPrint,&TCPServer::ProjectionIMGSignal,this,&Widget::ProjectionIMGSlot);

    //自动更新测试
    NetworkActiveCheck NetCheck;
    NetCheck.NetworkTest();
//    if(ConfigAndState::internetAlive)
//    {
//        qDebug()<<"开始检查更新";
//        update = new HTTPUpdate;
//        update->IsLastVersion("");
//        if(update->needUpdate)
//        {
//            ConfigAndState::SetIfNeedUpdate(true);
//        }
//        else
//        {
//            ConfigAndState::SetIfNeedUpdate(false);
//        }
////        connect(update,&HTTPUpdate::LoadProgressSignal,this,&Widget::LoadProgressSlot);
//    }


    //帧播放初始化
    ChangeImage = new QTimer();
    connect(ChangeImage,&QTimer::timeout,
            [&](){
        int nowFrame = ConfigAndState::GetFinishCount()-4;
//        qDebug()<<"now frame:"<<nowFrame;
        if(nowFrame>0)
        {
            if(frameChangeOpen)
            {
                if(frameChangeFlag == 0)
                {
                    frameImgPath = QString("../videoImg/%1.jpg").arg(nowFrame,5,10,QLatin1Char('0'));
                    frameChangeFlag = 1;
                    repaint();
                }
                if(frameChangeFlag == 1)
                {
                    frameImgPath = QString("../videoImg/%1.jpg").arg(nowFrame+1,5,10,QLatin1Char('0'));
                    frameChangeFlag = 0;
                    repaint();
                }
            }
            else
            {
                frameImgPath = QString("../videoImg/%1.jpg").arg(nowFrame,5,10,QLatin1Char('0'));
                frameChangeFlag = 1;
                repaint();
            }
        }

    });
    GetFrame = new TempGetFrame();
    connect(this,&Widget::GetAndShowFrameSignal,GetFrame,&TempGetFrame::GetFrameImg);

    connect(GetFrame,&TempGetFrame::SuccessGetFrameSignal,this,&Widget::ProcessFrameSlot);

    progressBar = new QProgressBar(this);
    progressBar->setVisible(false);



}

Widget::~Widget()
{
    UDPSendThread->quit();
    UDPSendThread->wait();
    TCPSendThread->quit();
    TCPSendThread->wait();
    ImageFrameThread->quit();
    ImageFrameThread->wait();

    delete printerConfig;
    delete sendUDPBoardcast;
    delete TCPServerInPrint;

    delete udpSendTimer;
    delete FrameSendTimer;
    delete ChangeImage;

    delete painter;

    delete ui;

    delete CheckUpdateWindow;
    delete AdjustPlaneWindow;
    delete AdjustPressWindow;
//    delete DeviceMessageWindow;
    delete WifiSearchWindow;

}

void Widget::UDPStart()
{
    emit UDPStartSignal(port,msg);
}
void Widget::TCPStart()
{
    qDebug()<<"开始tcp线程";

}

//打印控制
void Widget::PrintStartSlot()
{
    qDebug()<<"发送【开始】打印信号到主函数";
    emit StartPrintSignal();

}

void Widget::PrintContinueSlot()
{
    qDebug()<<"发送【继续】打印信号到主函数";
    emit ContinuePrintSignal();

}

void Widget::ProjectionIMGSlot(QString imgPath)
{
    qDebug()<<"发送【切换投影图片】信号到主函数";
    emit ProjectionIMGSignal(imgPath);
}

void Widget::ProcessFrameSlot(/*int frame,QImage image*/)
{
//    因为在3代树莓派上进行的测试，所以先注释掉这块
//        ChangeImage->start(500);
}

void Widget::paintEvent(QPaintEvent *)
//void Widget::paintGL()
{
    if(ConfigAndState::GetPrintState() == "stop")
    {
//        ui->PrintInformation->setVisible(true);
//        ui->StandbyPic->setVisible(true);
        frameChangeOpen = false;
        openFFmpeg = true;
    }
    else if(ConfigAndState::GetPrintState() == "pause")
    {
        frameChangeOpen = false;

    }
    else if(ConfigAndState::GetPrintState() == "continue" || ConfigAndState::GetPrintState() == "start")
    {
//        ui->PrintInformation->setVisible(false);
//        ui->StandbyPic->setVisible(false);
        QPainter painter(this);
        painter.drawPixmap(0, 0, QPixmap(frameImgPath));
        frameChangeOpen = true;
    }
}

void Widget::VideoReadySlot()       //准备阶段，收到解压完成的信号，就开始读取视频帧
{

    if(openFFmpeg)
    {
        openFFmpeg = false;
        qDebug()<<"收到子窗口帧播放信号";

        ImageFrameThread = new QThread();
        GetFrame->moveToThread(ImageFrameThread);
        ImageFrameThread->start();
        frameChangeOpen = true;
        emit GetAndShowFrameSignal();
    }
    else
    {
        ProcessFrameSlot();
    }

}

void Widget::LoadProgressSlot(qint64 bytesSent,qint64 bytesTotal)
{
//    ui->PrintInformation->setVisible(false);
//    ui->StandbyPic->setVisible(false);

    progressBar->setVisible(true);
    progressBar->setMaximum(bytesTotal);
    progressBar->setValue(bytesSent);
    progressBar->setFixedSize(1920,20);
    progressBar->move(0,0);
    progressBar->show();
}

void Widget::on_pressAdjust_clicked()
{
    if(!adjustPressWindowFlag)
    {

        AdjustPressWindow->show();
        adjustPressWindowFlag = true;
    }
    else
    {
        AdjustPressWindow->close();
        AdjustPressWindow->show();
    }
}

void Widget::on_update_clicked()
{
    if(!updateWindowFlag)
    {
        CheckUpdateWindow->show();
        updateWindowFlag = true;
    }
    else
    {
        CheckUpdateWindow->close();
        CheckUpdateWindow->show();
    }
}

void Widget::on_setHorizontal_clicked()
{

//    if(!adjustPlaneWindowFlag)
//    {
//        AdjustPlaneWindow->show();
//        adjustPlaneWindowFlag = true;
//    }
//    else
//    {
//        AdjustPlaneWindow->close();
//        AdjustPlaneWindow->show();
//    }
}

void Widget::on_deviceMessage_clicked()
{
    if(!messageWindowFlag)
    {
//        DeviceMessageWindow->show();
        messageWindowFlag = true;
    }
    else
    {
//        DeviceMessageWindow->close();
//        DeviceMessageWindow->show();
    }
}

void Widget::on_wifiSetting_clicked()
{
    if(!wifiSearchWindowFlag)
    {
//        WifiSearchWindow->show();
//        wifiSearchWindowFlag = true;
    }
    else
    {
//        WifiSearchWindow->close();
//        WifiSearchWindow->show();
    }
}
