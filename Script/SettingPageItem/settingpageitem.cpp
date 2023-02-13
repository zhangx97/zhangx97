#include "settingpageitem.h"

SettingPageItem::SettingPageItem(QObject *parent):QAbstractListModel(parent)
{
    roleNameToQml[functionNameRole] = "functionName";
    roleNameToQml[functionStateRole] = "functionState";
    InputItem();
}

SettingPageItem::~SettingPageItem(){

}

QVariant SettingPageItem::data(const QModelIndex &index, int role) const{
    int row = index.row();
    if(row<0 || row>settingList.size()){
        return QVariant();
    }
    Item *i = settingList[row];
    return i->at(role-Qt::UserRole);
}

int SettingPageItem::rowCount(const QModelIndex &parent) const{
    return settingList.count();
}

QHash<int,QByteArray> SettingPageItem::roleNames() const{
    return roleNameToQml;
}

void SettingPageItem::InputItem(){
    qDebug()<<"打开设置页面";
    settingItemDetail makeDetail;
    for (int i =0;i<endItem;i++) {
        Item *signalItem = new Item();
        if(i == deviceDetail)
        {
            signalItem->append("设备信息");
            signalItem->append("");
        }
        if(i == netState)
        {
            signalItem->append("网络");
            if(ConfigAndState::localNetAlive){
                signalItem->append("已连接");
            }else{
                signalItem->append("");
            }
        }
        if(i == material)
        {
            signalItem->append("材料");
            signalItem->append("");
        }
        if(i == update)
        {
            signalItem->append("检查更新");
            signalItem->append(""); //以后状态机里新增一个状态，然后补上
        }
        if(signalItem != 0){
            settingList.append(signalItem);
        }
        signalItem = 0;
    }
}
