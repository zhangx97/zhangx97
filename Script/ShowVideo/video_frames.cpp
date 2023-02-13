#include "Script/ShowVideo/video_frames.h"
#include <QDebug>
#include <QImage>
#include <QTime>

VideoFrames::VideoFrames(QObject *parent)
    : QAbstractVideoSurface(parent)
{
}

VideoFrames::~VideoFrames()
{
}

QList<QVideoFrame::PixelFormat> VideoFrames::supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const
{
    QList<QVideoFrame::PixelFormat> listPixelFormats;

    listPixelFormats<< QVideoFrame::Format_RGB32;

    //qDebug() << listPixelFormats;

    // Return the formats you will support
    return listPixelFormats;
}

bool  VideoFrames::present(const QVideoFrame &frame)
{
    //qDebug()<<"present启动";
    // Handle the frame and do your processing
    if (frame.isValid()) {

        QVideoFrame cloneFrame(frame);

        cloneFrame.map(QAbstractVideoBuffer::ReadOnly);
        QImage image(cloneFrame.bits(),
                    cloneFrame.width(),
                    cloneFrame.height(),
                    cloneFrame.bytesPerLine(),
                    QVideoFrame::imageFormatFromPixelFormat(cloneFrame.pixelFormat()));
        qDebug()<<"发送帧";
        emit frameAvailableSignal(image.copy());
        cloneFrame.unmap();
        return true;
    }
    else
    {
        qDebug()<<"无效帧";
        //emit PauseToSaveSignal();
    }
    //stop();
    return false;
}
