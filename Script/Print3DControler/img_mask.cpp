#include "Script/Print3DControler/img_mask.h"

ImgMask::ImgMask(QString maskPath,QString imgPath):maskPath(maskPath),projectPath(imgPath)
{
    ImageMask = QImage(maskPath);
    ImageProject = QImage(projectPath);
    NewImage = QImage(ImageMask.width(),ImageMask.height(),QImage::Format_ARGB32);
}

ImgMask::~ImgMask()
{
    delete  lineMask;
    delete  lineProject;
}

QImage ImgMask::MaskImg()
{
   QRgb * maskLine;
   QRgb * projectLine;

   for(int y = 0;y<ImageMask.height();y++)
   {
       maskLine = (QRgb *)ImageMask.scanLine(y);
       projectLine = (QRgb *)ImageProject.scanLine(y);
       for(int x = 0;x<ImageMask.width();x++)
       {
           int colorR = (qRed(maskLine[x])+qRed(projectLine[x]))/255;
           int colorG = (qGreen(maskLine[x])+qGreen(projectLine[x]))/255;
           int colorB = (qBlue(maskLine[x])+qBlue(projectLine[x]))/255;
           NewImage.setPixel(x,y,qRgb(colorR,colorG,colorB));
       }
   }
   return NewImage;
}
