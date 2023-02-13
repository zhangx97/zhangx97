#include "checkupdate.h"
#include "ui_checkupdate.h"

/*
 * 从widget转换为qml,用不到了
*/

CheckUpdate::CheckUpdate(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CheckUpdate)
{
    setWindowFlag(Qt::FramelessWindowHint);

    move(1920,0);
    ui->setupUi(this);
    ui->labelCheckingNow->show();
    ui->labelUpdateResult->hide();
    ui->progressBar->hide();
    ui->startUpdate->hide();

    if(ConfigAndState::internetAlive)
    {
        qDebug()<<"开始检查更新";
        update = new HTTPUpdate;
        update->IsLastVersion("http://realmaker.cc/cn/softwareversin3_1.aspx");

        if(update->needUpdate)
        {
            ui->startUpdate->show();
        }
        else
        {
            ui->labelCheckingNow->hide();
            ui->labelUpdateResult->setText("当前是最新版本，无需更新");
            ui->labelUpdateResult->show();
            ui->cancelUpdate->setText("返回");
        }
    }
}

CheckUpdate::~CheckUpdate()
{
    delete ui;
}

void CheckUpdate::on_cancelUpdate_clicked()
{
    this->close();
}

void CheckUpdate::LoadProgressSlot(qint64 bytesSent,qint64 bytesTotal)
{
//    ui->PrintInformation->setVisible(false);
//    ui->StandbyPic->setVisible(false);

    ui->progressBar->setVisible(true);
    ui->progressBar->setMaximum(bytesTotal);
    ui->progressBar->setValue(bytesSent);
//    ui->progressBar->setFixedSize(1920,20);
    ui->progressBar->move(0,0);
    ui->progressBar->show();
}

void CheckUpdate::on_startUpdate_clicked()
{
    update->Download("");
    connect(update,&HTTPUpdate::LoadProgressSignal,this,&CheckUpdate::LoadProgressSlot);
}
