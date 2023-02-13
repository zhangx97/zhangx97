#ifndef PROJECTPHOTO_H
#define PROJECTPHOTO_H

#include <QObject>
#include <QQuickPaintedItem>
#include <QImage>
#include <QPainter>
#include <QString>
#include "Script/StateMachine/config_and_state.h"

class ProjectPhoto:public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QString fileUrl READ getUrl WRITE setUrl)
public:
    ProjectPhoto();
    ~ProjectPhoto();

    virtual void paint(QPainter *painter);
//    Q_INVOKABLE void changePhotoSlot(QString photoPath);
    QString getUrl();
    void setUrl(QString path);

private:
//    QString imgPath;
    QString fileUrl = "../projection/blackscreen.png";
};

#endif // PROJECTPHOTO_H
