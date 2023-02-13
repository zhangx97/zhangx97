#ifndef GET_IMG_MESSAGE_H
#define GET_IMG_MESSAGE_H

#include <QObject>

class GetImgMessage:public QObject
{
    Q_OBJECT
public:
    void SendImgPath();

signals:
    void ImgPathChange(QString);
};

#endif // GET_IMG_MESSAGE_H
