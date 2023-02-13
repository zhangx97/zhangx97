#ifndef PROJECTIONWIDGET_H
#define PROJECTIONWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QApplication>
#include <QXmlStreamReader>
#include <QDateTime>
#include <QTimer>
#include <QPainter>
#include <QDebug>

#include "Script/StateMachine/config_and_state.h"
#include "Script/lightEngine/cyusbseriallib.h"
#include "Script/Print3DControler/img_mask.h"



class ProjectionWidget:public QWidget
{
    Q_OBJECT
public:
    ProjectionWidget(QWidget *parent=0);

    //ConfigAndState * PrintOrderFromStatus;
    QString projectionImgPath;


    void RefrshWindow();
    void ProjectionIMGSlot(QString &imgPath);

private:

    void paintEvent(QPaintEvent *);
    int PrintingCount;

};

#endif // PROJECTIONWIDGET_H
