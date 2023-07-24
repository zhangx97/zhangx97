#ifndef WIDGET_H
#define WIDGET_H

#include "Script/StateMachine/config_and_state.h"
#include "Script/Network/UDP/udpnetwork.h"
#include "Script/Network/TCP/tcpserver.h"
#include "Script/ShowVideo/video_player.h"
#include "Script/ShowVideo/temp_get_frame.h"
#include "Script/Network/HttpUpdate/http_update.h"
#include "Script/Network/NetworkActiveCheck/network_active_check.h"
//#include "Script/Pressure/pluse_calibrate.h"
#include "Script/CheckUpdate/checkupdate.h"
#include "AdjustPlane/adjustplane.h"
#include "AdjustPage/adjustpage.h"
#include "DeviceMessage/devicemessage.h"
#include "WifiSearch/wifisearch.h"
#include "Script/Print3DControler/xml_operation.h"

#include <QWidget>
#include <QTimer>
#include <QPainter>
#include <QImage>
#include <QPixmap>
#include <QBitmap>
#include <QMediaPlayer>
#include <QOpenGLWidget>
#include <QVideoWidget>
#include <QMap>
#include <QMutex>
#include <QProgressBar>

namespace Ui {
class Widget;
class CheckUpdate;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

    ConfigAndState *printerConfig;
    UDPNetwork *sendUDPBoardcast;
    TCPServer *TCPServerInPrint;
    HTTPUpdate *update;

    QTimer *udpSendTimer;
    QTimer *FrameSendTimer;
    QTimer *ChangeImage;

    QThread *UDPSendThread;
    QThread *TCPSendThread;
    QThread *ImageFrameThread;
    QThread *NetworkUpdate;

    QImage recvImage;

    QMap<int, QImage> FrameImage;
    QMap<int, QImage>::iterator it;

    TempGetFrame *GetFrame;

    void UDPStart();
    void TCPStart();
    void DealFrame();


    //发送打印信号的方法
    void PrintStartSlot();
    void PrintContinueSlot();
    void VideoReadySlot();      //收到开始打印信号，主界面准备图片投影
    void ProcessFrameSlot(/*int frame,QImage image*/);     //开始打印时，主界面显示实时帧
    void ProjectionIMGSlot(QString imgPath);        //调试工具用，切换光机投影图片

    void PauseShowFrameSlot();

    void LoadProgressSlot(qint64 bytesSent,qint64 bytesTotal);

protected:
    void paintEvent(QPaintEvent *);
//    void paintGL() override;



signals:
    void UDPStartSignal(int,QString);
    void TCPServerStartSignal(int);

    //打印控制的信号
    void StartPrintSignal();
    void ContinuePrintSignal();
    //图片投影信号
    void ProjectionIMGSignal(QString imgPath);
    //开始帧图像投影信号
    void GetAndShowFrameSignal();

    void StartCheckUpdate(QString);

private slots:
    void on_pressAdjust_clicked();

    void on_update_clicked();

    void on_setHorizontal_clicked();

    void on_deviceMessage_clicked();

    void on_wifiSetting_clicked();

private:
    Ui::Widget *ui;
    int port;
    QString id;
    QString msg;

    QPainter *painter;
    QMutex mutex;

    bool openFFmpeg = true;
    bool frameChangeOpen = true;
    int frameChangeFlag = 0;    //用于控制图片反复显示
    QString frameImgPath;

    CheckUpdate *CheckUpdateWindow = new CheckUpdate;
    AdjustPlane *AdjustPlaneWindow = new AdjustPlane;
    AdjustPage *AdjustPressWindow = new AdjustPage;
//    DeviceMessage *DeviceMessageWindow = new DeviceMessage;
    WifiSearch *WifiSearchWindow = new WifiSearch;
    bool adjustPlaneWindowFlag = false;
    bool updateWindowFlag = false;
    bool adjustPressWindowFlag = false;
    bool messageWindowFlag = false;
    bool wifiSearchWindowFlag = false;

    QProgressBar *progressBar;
};

#endif // WIDGET_H
