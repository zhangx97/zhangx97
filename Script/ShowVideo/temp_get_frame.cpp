#include "Script/ShowVideo/temp_get_frame.h"

TempGetFrame::TempGetFrame()
{

}

TempGetFrame::~TempGetFrame()
{

}

void TempGetFrame::GetFrameImg()
{
    QString videoCmd = QString("ffmpeg -i /home/pi/python_project/images/%1.mp4 "
                       "-r 25 -f image2 -qscale:v 2 /home/pi/python_project/videoImg/%05d.jpg -loglevel quiet")
            .arg(ConfigAndState::modelName.split(".")[0]);
    getFrameOrder = QString(videoCmd);
    GetFrameFlag = system(getFrameOrder.toUtf8());
    if (GetFrameFlag != -1)
    {

        emit SuccessGetFrameSignal();
    }
}
