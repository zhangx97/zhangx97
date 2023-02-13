#ifndef LOGCONTROL_H
#define LOGCONTROL_H

#include <QAbstractListModel>
#include <QHash>
#include <QVector>
#include <QFile>
#include <QTextStream>
#include <QDate>
#include <QDebug>

class LogControl:public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString choseDate READ GetChoseDate WRITE SetChoseDate)
public:
    enum LogInfo
    {
        time = Qt::UserRole,
        level,
        content,
        position,
        function,
        row
    };

    LogControl();
    ~LogControl();
    virtual int rowCount(const QModelIndex &parent) const override;     //返回model中数据的条目个数
    virtual QVariant data(const QModelIndex &index , int role) const override;      //获取某一行，某个角色对应的数据
    virtual QHash<int,QByteArray> roleNames() const;
    void createLogModel();//建立要往qml发送的模型
    QString GetChoseDate();
    void SetChoseDate(QString);
    Q_INVOKABLE void refresh();
    void Clear();

    QString logDate = QDate::currentDate().toString("yyyyMMdd");//保存qml界面中选中的日期
    QHash<int ,QByteArray> propertyName;

    QVector<QString> *singleLog;//单条日志信息
    QVector<QVector<QString> *> logModel;//单条日志的集合

};

#endif // LOGCONTROL_H
