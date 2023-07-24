#ifndef PRINT_CONTROL_H
#define PRINT_CONTROL_H

//#include "Script/Print3DControler/projection_widget.h"
#include <QObject>
#include <QXmlStreamReader>
#include <QString>

#include "Script/StateMachine/config_and_state.h"
#include "Script/Print3DControler/unzip.h"
#include "Script/Network/SerialPort/serialport.h"
#include "Script/lightEngine/cyusbseriallib.h"
#include "Script/Print3DControler/xml_operation.h"

class PrintControl:public QObject
{
    Q_OBJECT

public:
    PrintControl(QObject *parent = 0);
    ~PrintControl();

    Unzip *UnzipForPrint;
    bool ReadXML(QString &);//有空淘汰
    void ReadChildElement();//有空淘汰
    void ProjectionLayer();//控制投影层数
    void DealStartPrint(QString fileName);//控制开始打印
    void BuildNewLayer();//建立新的一层
    void ControlStop();//控制打印停止
    void ControlPause();//控制打印暂停
    void ControlContinue();//控制打印继续

    //属性
    QString photoPath();
    void SetPhotoPath(QString photoPath);

signals:
    void photoChange(QString photoPath);//向子窗口发送需要显示的图片
    void MachineIsStartPrintNowSignal();//控制帧进行播放的信号
    void UnzipFinish();
    void StopPrint();
    void PrintComplete();


private:
    QXmlStreamReader * reader;
    SerialPort * PrintCmd;
    CyUSBSerialLib * projection;
    XMLOperation * materialXml;

    QString projectionImgPath;//需要子窗口显示的图片路径
    int PrintingCount;
    int modelCount;
    int bottomCount;
//    QObject *m_root;
//    QObject *m_target;

    void SimulationPrint();
    void SimulationBuildNewLayer();
};
#endif // PRINT_CONTROL_H
