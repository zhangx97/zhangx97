#ifndef VIDEO_FRAMES_H
#define VIDEO_FRAMES_H

#include <QList>
#include <QVideoFrame>
#include <QAbstractVideoSurface>
#include <QMediaPlayer>
#include <QHash>

class VideoFrames : public QAbstractVideoSurface
{
    Q_OBJECT

public:
    VideoFrames(QObject *parent = Q_NULLPTR);
    ~VideoFrames();

    QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const;
    bool present(const QVideoFrame &frame) override;

    QVideoFrame *cloneFrame;

signals:
    void frameAvailableSignal(QImage image);

private:
    int frameIndex = 0;
    QHash<int,QImage> FrameImageHash;

};

#endif
