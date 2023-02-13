#ifndef QMLSTATE_H
#define QMLSTATE_H
#include <QObject>
#include <QString>
#include <QTimer>
#include "Script/StateMachine/config_and_state.h"

class QMLState:public QObject
{
    Q_OBJECT
public:
    QMLState();
    ~QMLState();

    Q_INVOKABLE void stop();
    Q_INVOKABLE void pause();
    Q_INVOKABLE void printContinue();

    Q_PROPERTY(QString printState READ GetPrintState NOTIFY printStateChanged)//打印状态
    Q_PROPERTY(double layerCount READ GetLayerCount NOTIFY layerCountChanged)//当前完成层数
    Q_PROPERTY(double finishCount READ GetFinishCount NOTIFY finishCountChanged)//总层数
    Q_PROPERTY(double printPercent READ GetPrintPercent NOTIFY printPercentChanged)//打印百分比

    Q_PROPERTY(double layerThickness READ GetLayerThickness NOTIFY layerThicknessChanged)//层厚
    Q_PROPERTY(QString remainingTime READ GetRemainTime NOTIFY remainTimeChanged)//剩余时间
    Q_PROPERTY(bool pauseFlag READ GetPauseFlag WRITE SetPauseFlag NOTIFY pauseFlagChanged)//暂停打印的标志位
    Q_PROPERTY(QString frontImg READ ReturnBackImg  NOTIFY frontImgGet)//模型俯视图
    Q_PROPERTY(QString backImg READ ReturnBackImg  NOTIFY backImgGet)//模型仰视图
    Q_PROPERTY(QString machineSerialNum READ ReturnNum NOTIFY printIDChange)//机器序列号

    Q_PROPERTY(double resinTemperature READ GetResinTemp NOTIFY resinTempChanged)//树脂槽温度
    Q_PROPERTY(int ledBrightness READ GetLEDBrightness NOTIFY ledBrightnessChanged)//led亮度
    Q_PROPERTY(int ledCurrent READ GetLEDCurrent NOTIFY ledCurrentChanged)//led电流大小
    Q_PROPERTY(double ledTemperature READ GetLEDTemp NOTIFY ledTempChanged)//led温度

    QString nowPrintState = "";//当前打印状态
    double allLayerCount = 0;//总层数
    double nowFinishCount = 0;//当前完成层数
    double percent = 0.0;//打印的百分比
    double layThick = 0.0;//层厚

    QString remainTime = "";//剩余时间
    bool pauseFlag = false;//true暂停，false继续
    QString machineSerialID = "";//设备序列号
    double resinTemp = 0.0;//树脂槽温度
    int ledBright = 0;//led亮度
    int ledCurrentValue = 0;//led电流
    double ledTemp = 0.0;//led温度

    QTimer *updateTimer;

    QString GetPrintState();
    double GetLayerCount();
    double GetFinishCount();
    double GetPrintPercent();
    double GetLayerThickness();
    QString GetRemainTime();
    bool GetPauseFlag();
    void SetPauseFlag(bool flag);
    QString ReturnFrontImg();
    QString ReturnBackImg();
    QString ReturnNum();

    double GetResinTemp();
    int GetLEDBrightness();
    int GetLEDCurrent();
    double GetLEDTemp();

    void UpdateSecond();

signals:
    void printStateChanged(const QString &printState);
    void layerCountChanged(const int &LayerCount);
    void finishCountChanged(const int &FinishCount);
    void printPercentChanged(const double &printPercent);
    void layerThicknessChanged(const double &LayerThickness);
    void remainTimeChanged(const QString &RemainTime);
    void pauseFlagChanged(const bool &pauseFlag);// (暂停/继续) 的状态改变时发送的信号。
    void frontImgGet(const QString &printState);
    void backImgGet(const QString &printState);
    void printIDChange(const QString &printID);

    void resinTempChanged(const double &ResinTemp);
    void ledBrightnessChanged(const int &ledBright);
    void ledCurrentChanged(const int &ledCurrent);
    void ledTempChanged(const double &ledTemp);
};

#endif // QMLSTATE_H
