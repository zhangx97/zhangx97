#include "projectphoto.h"

ProjectPhoto::ProjectPhoto()
{
    update();
}

ProjectPhoto::~ProjectPhoto()
{

}

void ProjectPhoto::paint(QPainter *painter)
{
    if (painter == nullptr)
    {
        return;
    }
    QImage sourceImg;
    sourceImg.load(fileUrl);
    sourceImg = sourceImg.convertToFormat(QImage::Format_Grayscale8);

    QImage maskImg;
    QString maskPath = QString("../mask/%1.png").arg(QString(ConfigAndState::lightengineSN));
//    QString maskPath = "../mask/test.jpg";
    maskImg.load(maskPath);
    maskImg = maskImg.convertToFormat(QImage::Format_Grayscale8);

    painter->drawImage(QPoint(0, 0), sourceImg);
    painter->setCompositionMode(QPainter::CompositionMode_Multiply);//设置混合
    painter->drawImage(QPoint(0, 0), maskImg);
}

QString ProjectPhoto::getUrl()
{
    return fileUrl;
}

void ProjectPhoto::setUrl(QString path)
{
    fileUrl = path;
    if(fileUrl == "")
    {
        fileUrl = "../projection/blackscreen.png";
    }
    update();
}
