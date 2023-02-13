#include "wificonnect.h"
#include "ui_wificonnect.h"

wifiConnect::wifiConnect(QString connectMode ,QWidget *parent,int custom) :
    QWidget(parent),
    ui(new Ui::wifiConnect),
    connectMode(connectMode)
{
    ui->setupUi(this);
    ui->label_3->setVisible(true);

    if(custom == 0)
    {
        ui->wifiName->setEnabled(false);
        ui->wifiPassword->installEventFilter(this);
    }
    else
    {
        ui->wifiName->setEnabled(true);
        ui->wifiName->installEventFilter(this);
        ui->wifiPassword->installEventFilter(this);
    }
}

wifiConnect::~wifiConnect()
{
    delete ui;
}

void wifiConnect::setWifiName(QString name)
{
    ui->wifiName->setText(name);
}


bool wifiConnect::eventFilter(QObject *watched, QEvent *event)
{
    if ( ((watched == ui->wifiName)||(watched == ui->wifiPassword)) && (event->type() == QEvent::MouseButtonPress) )
    {
        qDebug()<<"事件发生";
        CallKeyBoard((QWidget *)watched);
    }
    return QWidget::eventFilter(watched,event);
}


void wifiConnect::CallKeyBoard(QWidget *watched)
{
//    keyBoard->receiver = watched;

//    if(!keyboardShow)
//    {
////        AeaQt::Keyboard *keyBoard = new AeaQt::Keyboard(watched);
//        keyBoard->show();
//        keyboardShow = true;
//    }
//    else
//    {
//        keyBoard->close();
//        keyBoard->show();
//        keyboardShow = false;
//    }

}

void wifiConnect::on_cancel_clicked()
{
    this->close();
}

void wifiConnect::on_connectWifi_clicked()
{
    QString filePath = "/etc/wpa_supplicant/wpa_supplicant.conf";
    QFile writeFile(filePath);
    if(!writeFile.open(QIODevice::WriteOnly|QIODevice::Append|QIODevice::Text))
    {
        qDebug()<<"打开wifi文件失败";
    }
    else
    {
        QString name = ui->wifiName->text();
        QString password = ui->wifiPassword->text();
        QTextStream fileStream(&writeFile);
        QString message = QString("network={\n\tssid=\"%1\"\n\tpsk=\"%2\"\n\tkey_mgmt=WPA-PSK\n}").arg(name,password);
        fileStream<<message<<endl;
        writeFile.close();
        qDebug()<<"write sucess";

        system("sudo wpa_cli -i wlan0 reconfigure");

        //这里显示一个正在连接的界面
        ui->connectWifi->setVisible(false);
        ui->cancel->setVisible(false);
        ui->label->setVisible(false);
        ui->label_2->setVisible(false);
        ui->wifiName->setVisible(false);
        ui->wifiPassword->setVisible(false);
        ui->label_3->setVisible(true);
        //

        Delay_MSec(3000);

        //关闭那个界面

        //

        NetworkActiveCheck check;
        check.NetworkTest();
        if(ConfigAndState::localNetAlive)
        {
            qDebug()<<"连接成功";
            ui->label_3->setText("连接成功");
            Delay_MSec(3000);
            this->close();
        }
    }
}

void wifiConnect::Delay_MSec(unsigned int msec)
{
    QTime _Timer = QTime::currentTime().addMSecs(msec);
        while( QTime::currentTime() < _Timer )
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

