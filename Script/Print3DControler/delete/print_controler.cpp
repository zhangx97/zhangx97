#include "Script/Print3DControler/print_controler.h"

PrintControler::PrintControler()
{
//    ScreenShow = new ProjectionWidget;
    Motor = new MotorMove();
    ProjectThread = new QThread();
    Motor->moveToThread(ProjectThread);
    ProjectThread->start();

    connect(this,&PrintControler::StartPrintSignal,Motor,&MotorMove::DealStartPrint);
    connect(this,&PrintControler::ContinuePrintSignal,Motor,&MotorMove::ControlContinue);
    connect(Motor,&MotorMove::ChangePhoto,this,&PrintControler::RefreshSubWidgetSlot);
    //播放时控制帧切换
    connect(Motor,&MotorMove::MachineIsStartPrintNowSignal,this,&PrintControler::MachineIsStartPrintNowSlot);
//    connect(Motor,&MotorMove::StopShowFrameSignal,this,&PrintControler::StopShowFrameSlot);
//    connect(Motor,&MotorMove::PauseShowFrameSignal,this,&PrintControler::PauseShowFrameSlot);


}

PrintControler::~PrintControler()
{
    ProjectThread->quit();
    ProjectThread->wait();

    delete Motor;
//    delete ScreenShow;
}

void PrintControler::ControlStartSlot()
{
    emit StartPrintSignal();
}

void PrintControler::ControlContinueSlot()
{
    emit ContinuePrintSignal();
}

void PrintControler::ProjectionIMGSlot(QString &imgPath)
{

//    emit ProjectionIMGSignal(imgPath);
//    connect(this,&PrintControler::ProjectionIMGSignal,ScreenShow,&ProjectionWidget::ProjectionIMGSlot);
}

void PrintControler::MachineIsStartPrintNowSlot()
{
    qDebug()<<"子窗口打印控制发出帧播放信号";
    emit MachineIsStartPrintNowSignal();
}

void PrintControler::RefreshSubWidgetSlot()
{
    emit RefreshSubWidgetSignal();
}

void PrintControler::StopShowFrameSlot()
{
    emit StopShowFrameSignal();
}

void PrintControler::PauseShowFrameSlot()
{
    emit PauseShowFrameSignal();
}
