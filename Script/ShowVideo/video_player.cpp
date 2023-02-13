#include "Script/ShowVideo/video_player.h"

VideoPlayer::VideoPlayer(int distributionNumber):mediaPlayer(new QMediaPlayer),videoFrames(new VideoFrames),distributionDealFrame(distributionNumber)
{
    mediaPlayer->setVideoOutput(videoFrames);
    mediaPlayer->setMedia(QUrl(QUrl::fromLocalFile(QStringLiteral("/home/pi/learnQt/build-3DPrintLayout-unknown-Debug/video/test.mp4"))));
    connect(mediaPlayer,&QMediaPlayer::mediaStatusChanged,this,&VideoPlayer::FindoutMediaStatusSlot);

    connect(mediaPlayer, &QMediaPlayer::durationChanged, this, [&](qint64 duration) {
        qDebug() << "Media duration = " << duration;
        videoGetReady = true;
    });

    connect(videoFrames, &VideoFrames::frameAvailableSignal, this, &VideoPlayer::GetFrameImageSlot);
}

VideoPlayer::~VideoPlayer()
{

}

void VideoPlayer::GetAndShowFrameSlot(int frame)
{
//    qDebug()<<"帧播放线程的ID:"<<QThread::currentThreadId()<<"\n";
    if (frame%2 == distributionDealFrame && videoGetReady)
    {
//        qDebug()<<"处理帧："<<frame;

        showFrame = frame;
        qDebug()<<"改变位置";
        mediaPlayer->setPosition(frame*40);
        mediaPlayer->play();
        mediaPlayer->pause();
    }
}

void VideoPlayer::GetFrameImageSlot(QImage image)
{
    qDebug()<<"VideoPlayer,向主窗口发送信号";

    emit SendFrameImageSignal(showFrame, image);
}

void VideoPlayer::PauseVideoSlot()
{
//    qDebug()<<"帧暂停";
    mediaPlayer->pause();
    mediaPlayer->play();
}

void VideoPlayer::FindoutMediaStatusSlot(QMediaPlayer::MediaStatus status)
{
    qDebug() << "Media Status:" << status;
}

