#include "Script/Print3DControler/projection_widget.h"


ProjectionWidget::ProjectionWidget(QWidget *parent):QWidget(parent)
{
    //setWindowFlags(Qt::FramelessWindowHint);

    //PrintOrderFromStatus = new ConfigAndState();
    ConfigAndState::resetSubWidget = true;      //true时可以重置投影窗口为黑色
    if(ConfigAndState::resetSubWidget == true)
    {
        update();
    }
}

void ProjectionWidget::RefrshWindow()
{
    PrintingCount = ConfigAndState::GetFinishCount();
    qDebug()<<"开始重绘:"<<PrintingCount;
    projectionImgPath = QString("../images/%1.png").arg(PrintingCount,4,10,QLatin1Char('0'));
    repaint();
}

void ProjectionWidget::paintEvent(QPaintEvent *)
{
    if(ConfigAndState::resetSubWidget == true)
    {
        move(0,40);
        resize(1920,1080);
        setStyleSheet("background-color:rgb(0,0,0)");
    }else
    {
        QPainter p(this);    

//        ImgMask ImgToProject(ConfigAndState::mask_img,projectionImgPath);     //正式打印时使用该注释部分
//        QImage image =  ImgToProject.MaskImg();
//        QPixmap tempPixmap = QPixmap::fromImage(image);
//        p.drawPixmap(0, 0, tempPixmap);

        qDebug()<<"切换投影图片："<<projectionImgPath;
        p.drawPixmap(0, 0, projectionImgPath);

        //PrintingCount++;
    }
}

void ProjectionWidget::ProjectionIMGSlot(QString &imgPath)
{
    ConfigAndState::resetSubWidget = false;
    projectionImgPath = imgPath;
    repaint();
    CyUSBSerialLib ProjectImg(ConfigAndState::lightSensorType);
    ProjectImg.AutoSetValue(ConfigAndState::targetLight,ConfigAndState::lightValueOffset);
}
