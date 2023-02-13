#include "adjustpage.h"
#include "ui_AdjustPage.h"
#include "Script/Pressure/pluse_calibrate.h"

AdjustPage::AdjustPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AdjustPage)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    move(1920,0);
}

AdjustPage::~AdjustPage()
{
    delete ui;
}

void AdjustPage::on_startAdjust_clicked()
{
    PluseCalibrate CheckPluse;
}

void AdjustPage::on_cancel_clicked()
{
    this->close();
}
