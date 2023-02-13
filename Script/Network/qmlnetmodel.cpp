#include "qmlnetmodel.h"

qmlNetModel::qmlNetModel(QObject *parent):QAbstractListModel(parent)
{
    qmlNetRoleName[localNet] = "localNet";
    qmlNetRoleName[internetNet] = "internetNet";
    qmlNetStateInit();
    CheckNet = new QTimer();
}

bool qmlNetModel::GetLocalActive(){
    CheckNet->start(0);
    qDebug()<<"查询局域网";
    connect(CheckNet,&QTimer::timeout,[=]()
    {
        qDebug()<<"显示局域网是否连接";
        NetworkActiveCheck check;
        check.LocalNetTest();
        CheckNet->stop();
    });

    return ConfigAndState::localNetAlive;
}

bool qmlNetModel::GetOuterActive(){
    CheckNet->start(0);
    qDebug()<<"查询互联网";
    connect(CheckNet,&QTimer::timeout,[=]()
    {
        NetworkActiveCheck check;
        check.InternetTest();
        CheckNet->stop();
    });

    return ConfigAndState::internetAlive;
}

void qmlNetModel::qmlNetStateInit(){
    for (int i=0;i<endline;i++) {
        if(i == localNet){
            qmlNetState.append(ConfigAndState::localNetAlive);
        }
        if(i == internetNet){
            qmlNetState.append(ConfigAndState::internetAlive);
        }
    }
}

QHash<int,QByteArray> qmlNetModel::roleNames() const{
    return qmlNetRoleName;
}

int qmlNetModel::rowCount(const QModelIndex &parent) const{
    return qmlNetState.count();
}

QVariant qmlNetModel::data(const QModelIndex &index, int role) const{
    int row = index.row();
    if(row<0 || row>qmlNetState.count()){
        return QVariant();
    }
    return qmlNetState[row-Qt::UserRole];
}
