#include "opendoor.h"
#include <QDebug>
#include <QIODevice>

OpenDoor::OpenDoor(QObject *parent) : QObject(parent)
{

}

void OpenDoor::dooropenbutton_clicked()
{
    qDebug() << __FUNCTION__;
    process.start("sudo python /home/pi/python_project/config/test.py\n");
    process.waitForStarted();
}
