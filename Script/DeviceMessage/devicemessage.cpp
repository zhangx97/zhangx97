#include "devicemessage.h"
#include "ui_devicemessage.h"

DeviceMessage::DeviceMessage(QObject *parent) :
     QAbstractListModel(parent)
{
    roleName[infoName]="infoName";
    roleName[infoContent] = "infoContent";
    roleName[infoSection] = "infoSection";
    GetDeviceInfo();
}

DeviceMessage::~DeviceMessage()
{
}

int DeviceMessage::rowCount(const QModelIndex &parent) const{
    return deviceInfo.length();
}

QVariant DeviceMessage::data(const QModelIndex &index, int role) const{
    int row = index.row();
    if(row<0 || row >deviceInfo.count()){
        return QVariant();
    }
    item *i = deviceInfo[row];
    return i->at(role-Qt::UserRole);
}

QHash<int,QByteArray> DeviceMessage::roleNames() const{
    return roleName;
}

void DeviceMessage::clear()
{
    int count = deviceInfo.count();
    if(count>0)
    {
        for(int i = 0;i<count;i++)
        {
            delete deviceInfo.at(i);
        }
        deviceInfo.clear();
    }
}

void DeviceMessage::refresh()
{
    beginResetModel();
    clear();
    GetDeviceInfo();
    endResetModel();
}

void DeviceMessage::GetDeviceInfo(){
    qDebug()<<"显示设备信息";
    for (int i=0;i<endline;i++) {
        item *deviceType = new item();
        if(i == rmkid){
            deviceType->append("设备序列号");
            deviceType->append(ConfigAndState::printID);
            deviceType->append("device");
        }
        if(i == rmkType){
            deviceType->append("设备型号");
            deviceType->append("机型【"+QString::number(ConfigAndState::modelType)+"】");
            deviceType->append("device");
        }
        if(i == LEDID){
            deviceType->append("光机序列号");
            deviceType->append(ConfigAndState::lightengineSN);
            deviceType->append("lightMachine");
        }
        if(i == LEDType){
            deviceType->append("光机型号");
            deviceType->append("【"+QString::number(ConfigAndState::lightSensorType)+"】型");
            deviceType->append("lightMachine");
        }
        if(i == ip){
            deviceType->append("当前ip");
            deviceType->append(ConfigAndState::TcpHostIp);
            deviceType->append("network");
        }
        if(i == systemVersion){
            deviceType->append("系统版本");
            deviceType->append(ConfigAndState::version);
            deviceType->append("system");
        }
        if(i == sleepTime)
        {
            deviceType->append("休眠时间");
            deviceType->append(QString::number(ConfigAndState::sleepTime)+"分钟");
            deviceType->append("sleepTime");
        }
        if(i == heatSwitch)
        {
            deviceType->append("加热开关");
            deviceType->append(ConfigAndState::heaterSwitch);
            deviceType->append("heat");
        }
        deviceInfo.append(deviceType);
        deviceType = 0;
    }
}
