import QtQuick 2.0
//设置图标
//功能按钮命名规范：如系统设置：
//这里只显示图标按钮，相应的窗口从main中通过信号实现
/*
  最外层的方形部件就叫做：<功能名>LogoRec
  存放图标的部件：<功能名>Icon
  显示应用名的部件：<功能名>IconName
*/

Item {
    width:170
    height: 240
    
    Rectangle{
        id:settingLogoRec
        anchors.top: parent.top
        radius: 50
        width:170
        height: 170

        Image{
            id:settingIcon
            anchors.fill: parent
            source: "qrc:/img/img/setting.png"
        }
    }
    Text {
        id: settingIconName
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter

        horizontalAlignment: Text.AlignHCenter
        font.pointSize: 16
        text: qsTr("设备信息")
    }
}
