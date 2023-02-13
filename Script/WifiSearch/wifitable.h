#ifndef WIFITABLE_H
#define WIFITABLE_H

#include <QHeaderView>

#include <QPainter>

#include <QMouseEvent>

class WifiTable:public QHeaderView
{
    Q_OBJECT
public:
    explicit WifiTable(Qt::Orientation orientation, QWidget *parent = 0);
    ~WifiTable();

signals:
    void headCheckBoxToggled(bool checked);

protected:
       void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const;
       void mousePressEvent(QMouseEvent *event);
       void mouseMoveEvent(QMouseEvent *event);

private:
       bool m_isOn;//是否选中
       QPoint m_mousePoint;//鼠标位置
       mutable QRect m_RectHeaderCheckBox;//复选框的位置

};



#endif // WIFITABLE_H
