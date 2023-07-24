#ifndef MESSAGE_DEAL_H
#define MESSAGE_DEAL_H

#include <QTcpSocket>
#include <QThread>
#include <QMutex>
#include <QThread>
#include <QFileInfo>
#include "Script/StateMachine/config_and_state.h"
#include "Script/lightEngine/cyusbseriallib.h"
#include "Script/Print3DControler/xml_operation.h"
#include "Script/Network/SerialPort/serialport.h"

class MessageDeal:public QObject
{
    Q_OBJECT
public:
    MessageDeal();
    ~MessageDeal();

    QTcpSocket *TcpSocket;
    void DealOrder(QString,qintptr);
    void UTF8ToGB2312(QByteArray &code);
    void ReturnStateToPC(qintptr);
    void ReturnHelpMessage(qintptr);
    void RestartProgram();
    void ShutdownProgram();

    CyUSBSerialLib *lightengine;

signals:
    void StartPrintSignal(QString filePath);
    void PausePrintSignal();
    void ContinuePrintSignal();
    void StopPrintSignal();
    void OrderResultSignal(QByteArray,qintptr);
    void FileTransSignal(QByteArray,qintptr);
    void ProjectionIMG(QString imgPath);


private:
    QMutex mutex;
    QString messageNeedToSend;
    bool Upload(QString fileName);
    void AdjustPlatform(QByteArray cmd);
    void GetVersion(qintptr);
    void ChangeParament(QString,QString);
    void PluseCalibrate();
    bool InitPrint(QString packName);//准备打印需要的参数
    int fileSize;
    QString filePath;//相对路径../slicepack/.....
    QString fileName;
};

#endif // MESSAGE_DEAL_H
