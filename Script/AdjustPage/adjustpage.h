#ifndef ADJUSTPAGE_H
#define ADJUSTPAGE_H

#include <QWidget>

namespace Ui {
class AdjustPage;
}

class AdjustPage : public QWidget
{
    Q_OBJECT

public:
    explicit AdjustPage(QWidget *parent = nullptr);
    ~AdjustPage();

private slots:
    void on_startAdjust_clicked();

    void on_cancel_clicked();

private:
    Ui::AdjustPage *ui;
};

#endif // ADJUSTPAGE_H
