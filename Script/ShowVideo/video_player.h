#ifndef VIDEO_PLAYER_H
#define VIDEO_PLAYER_H

#include <QMediaPlayer>
#include <QTimer>

#include "Script/ShowVideo/video_frames.h"
#include "Script/StateMachine/config_and_state.h"

class VideoPlayer:public QObject
{
    Q_OBJECT
public:
    VideoPlayer(int distributionNumber);
    ~VideoPlayer();


    QMediaPlayer *mediaPlayer;
    VideoFrames *videoFrames;

    void GetAndShowFrameSlot(int frame);
    void GetFrameImageSlot(QImage);
    void PauseVideoSlot();
    void FindoutMediaStatusSlot(QMediaPlayer::MediaStatus status);

signals:
    void SendFrameImageSignal(int frame,QImage image);
    void VideoLoadedSignal();

private:
    int showFrame=0;
    int distributionDealFrame;
    bool videoGetReady = false;

};


#endif // VIDEO_PLAYER_H
