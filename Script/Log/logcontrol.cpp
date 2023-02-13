#include "logcontrol.h"

LogControl::LogControl()
{
    propertyName[time] = "time";
    propertyName[level] = "level";
    propertyName[content] = "content";
    propertyName[position] = "position";
    propertyName[function] = "function";
    propertyName[row] = "row";
    createLogModel();
}
LogControl::~LogControl()
{
    Clear();
    if(singleLog != 0)
    {
        delete singleLog;
    }
}
void LogControl::refresh()
{
    beginResetModel();
    Clear();
    createLogModel();
    endResetModel();
}
void LogControl::Clear()
{
    int count = logModel.count();
    if(count>0)
    {
        for (int i = 0;i<count;i++)
        {
            delete logModel.at(i);
        }
        logModel.clear();
    }
}
void LogControl::createLogModel()
{
    qDebug()<<"开始查看日志";
    QString filePath = "../logs/"+logDate+".txt";
    QString logContent;
    QStringList singleLogList;

    QFile ReadFile(filePath);
    if(!ReadFile.open(QIODevice::ReadOnly |QIODevice::Text))
    {
        qDebug()<<"日志文件打开失败";
        return;
    }
    QTextStream ReadFileText(&ReadFile);
    logContent = ReadFileText.readAll();
    singleLogList = logContent.split("@@\n");
    //切分文件后二次处理
    for(int i =0;i<singleLogList.count();i++)
    {
        singleLog = new QVector<QString>();
        QStringList singleLogDetail;
        singleLogDetail = singleLogList[i].split("@@");
        if(singleLogDetail.count() == 6)
        {
            for(int j = 0;j<singleLogDetail.count();j++)
            {
                singleLog->append(singleLogDetail[j]);
            }
            logModel.append(singleLog);
            singleLog = 0;
        }
    }
}
int LogControl::rowCount(const QModelIndex &parent) const
{
    return logModel.count();
}
QHash<int,QByteArray> LogControl::roleNames() const
{
    return propertyName;
}
QVariant LogControl::data(const QModelIndex &index, int role) const
{
    int returnRow = index.row();
    if(returnRow<0||returnRow>logModel.size())
    {
        return QVariant();
    }
    QVector<QString> *logDate = logModel.at(returnRow);

    switch(role)
    {
    case time:
    {
        return logDate->at(role-Qt::UserRole);
    }
    case level:
    {
        return logDate->at(role-Qt::UserRole);
    }
    case content:
    {
        return logDate->at(role-Qt::UserRole);
    }
    case position:
    {
        return logDate->at(role-Qt::UserRole);
    }
    case function:
    {
        return logDate->at(role-Qt::UserRole);
    }
    case row:
    {
        return logDate->at(role-Qt::UserRole);
    }
    default:
    {
        return QVariant();
    }
    }

}
QString LogControl::GetChoseDate()
{
    return logDate;
}
void LogControl::SetChoseDate(QString date)
{
    logDate = date;
}
