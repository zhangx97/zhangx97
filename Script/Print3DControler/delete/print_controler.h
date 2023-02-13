#ifndef PRINT_CONTROLER_H
#define PRINT_CONTROLER_H

#include <QThread>
#include "Script/Print3DControler/print_control.h"
//#include "Script/Print3DControler/projection_widget.h"

class PrintControler:public QObject
{
    Q_OBJECT

public:
    PrintControler();
    ~PrintControler();

    MotorMove *Motor;
    QThread *ProjectThread;
//    ProjectionWidget *ScreenShow;

    void ControlStartSlot();
    void ControlContinueSlot();
    void ProjectionIMGSlot(QString &imgPath);
    void MachineIsStartPrintNowSlot();
    void RefreshSubWidgetSlot();

    void StopShowFrameSlot();
    void PauseShowFrameSlot();

signals:
    void StartPrintSignal();
    void ContinuePrintSignal();
    void ProjectionIMGSignal(QString &imgPath);
    void MachineIsStartPrintNowSignal();
    void RefreshSubWidgetSignal();

    void StopShowFrameSignal();
    void PauseShowFrameSignal();
    
};

#endif // PRINT_CONTROLER_H
