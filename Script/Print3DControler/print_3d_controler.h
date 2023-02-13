#ifndef PRINT3DCONTROLER_H
#define PRINT3DCONTROLER_H

#include <QObject>

#include "head/Print3DControler/unzip.h"
#include "head/Print3DControler/xml_operation.h"
#include "head/Print3DControler/projection_widget.h"
#include "head/config_and_state.h"
#include "head/Print3DControler/serialport.h"

class Print3DControler:public QObject
{
    Q_OBJECT
public:
    Unzip UnzipFile;
    XMLOperation ReadXMLFile;
    ProjectionWidget ShowImgToLabel;
    ConfigAndState PrintConfig;
    SerialPort SendMessageToMoveMachine;

    void SendImgFileName();


signals:

private:

};

#endif // PRINT3DCONTROLER_H
