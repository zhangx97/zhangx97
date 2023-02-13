#include "Script/Pressure/pluse_calibrate.h"

PluseCalibrate::PluseCalibrate()
{
    SerialPort PluseCalibrate;
    PluseCalibrate.SendPort("G15\n");
    ConfigAndState::pluseCount = 1300000;
    bool m18 = PluseCalibrate.SendPort("M18\n");
    bool g28;
    if(m18)
    {
        qDebug()<<"已经回到最高点，开始校正压力";
        g28 = PluseCalibrate.SendPort("G28\n");
    }
}
