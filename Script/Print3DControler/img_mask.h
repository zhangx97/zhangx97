#ifndef IMG_MASK_H
#define IMG_MASK_H
#include <QString>
#include <QImage>
#include <QRgb>

class ImgMask
{
public:
    ImgMask(QString,QString);
    ~ImgMask();

    QImage ImageMask;
    QImage ImageProject;
    QImage MaskImg();

private:
    QImage NewImage;
    QString maskPath;
    QString projectPath;
    uchar* lineMask;
    uchar* lineProject;
};

#endif // IMG_MASK_H
