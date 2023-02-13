#ifndef SCREENPROTECT_H
#define SCREENPROTECT_H

#include <QObject>
#include <QGuiApplication>
#include <QTimer>
#include <QEvent>
#include <QDebug>
#include <QString>

#include "Script/Print3DControler/xml_operation.h"
#include "Script/StateMachine/config_and_state.h"

class ScreenProtect : public QObject
{
    Q_OBJECT
public:
    explicit ScreenProtect(QObject *parent = nullptr);
    ~ScreenProtect();
    bool eventFilter(QObject *watched, QEvent* event) override;
    void showScreenProtect();
    Q_INVOKABLE void setScreenProtectTime(QString time);

    QTimer *screenCheckTimer;
signals:
    void startBlack();

public slots:
};

#endif // SCREENPROTECT_H
