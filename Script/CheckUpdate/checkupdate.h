#ifndef CHECKUPDATE_H
#define CHECKUPDATE_H

/*
 * 从widget转换为qml,用不到了
*/

#include <QWidget>
#include "Script/Network/HttpUpdate/http_update.h"
#include "Script/Network/NetworkActiveCheck/network_active_check.h"
#include "Script/Print3DControler/xml_operation.h"

namespace Ui {
class CheckUpdate;
}

class CheckUpdate : public QWidget
{
    Q_OBJECT

public:
    explicit CheckUpdate(QWidget *parent = nullptr);
    ~CheckUpdate();

private slots:
    void on_cancelUpdate_clicked();

    void on_startUpdate_clicked();

    void LoadProgressSlot(qint64 bytesSent,qint64 bytesTotal);

private:
    Ui::CheckUpdate *ui;
    HTTPUpdate *update;

};

#endif // CHECKUPDATE_H
