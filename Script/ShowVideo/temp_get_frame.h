#ifndef TEMP_GET_FRAME_H
#define TEMP_GET_FRAME_H

#include <stdlib.h>
#include <QObject>
#include <QDebug>

#include "Script/StateMachine/config_and_state.h"

class TempGetFrame:public QObject
{
    Q_OBJECT

public:
    TempGetFrame();
    ~TempGetFrame();

    void GetFrameImg();

signals:
    void SuccessGetFrameSignal();

private:
    QString getFrameOrder;
    int GetFrameFlag;
};

#endif // TEMP_GET_FRAME_H
