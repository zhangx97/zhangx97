#include "screenprotect.h"

ScreenProtect::ScreenProtect(QObject *parent) : QObject(parent)
{
    QGuiApplication::instance()->installEventFilter(this);
    screenCheckTimer = new QTimer();
    screenCheckTimer->start(300000);
    connect(screenCheckTimer,&QTimer::timeout,this,&ScreenProtect::showScreenProtect);
}

ScreenProtect::~ScreenProtect()
{
    delete screenCheckTimer;
}

bool ScreenProtect::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type() == QEvent::MouseMove ||
            event->type() == QEvent::MouseButtonPress ||
            event->type() == QEvent::MouseButtonRelease ||
            event->type() == QEvent::MouseTrackingChange)
    {
        screenCheckTimer->start(300000);
    }
    return QObject::eventFilter(watched,event);
}

void ScreenProtect::showScreenProtect()
{
    emit startBlack();
}

void ScreenProtect::setScreenProtectTime(QString time)
{
    XMLOperation config("../config/realmakerSetting.config");
    bool successFlag = false;
    successFlag = config.WriteXMLData("休眠时间",time.split("分钟")[0]);
    ConfigAndState::sleepTime = time.split("分钟")[0].toInt();
    if(successFlag)
    {
        qDebug()<<"设置休眠时间为:"<<time;
    }
    else
    {
        qDebug()<<"设置休眠时间失败";
    }
}
