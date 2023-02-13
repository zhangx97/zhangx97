import QtQuick 2.0
import QtGraphicalEffects 1.0
//wifi图标
//这里只显示图标按钮，相应的窗口从main中通过信号实现
//功能按钮命名规范：以系统设置为例：
/*
  最外层的方形部件就叫做：<功能名>LogoRec
  存放图标的部件：<功能名>Icon
  显示应用名的部件：<功能名>IconName
  用来加载的部件：<功能名>Loder
*/

Item {
    width:170
    height: 240

    Rectangle{
        id:wifiLogoRec
        anchors.top: parent.top
        radius: 50
        width:170
        height: 170

        Image{
            id:wifiIcon
            anchors.fill: parent
            source: "qrc:/img/img/wifi.png"
        }
    }
    Text {
        id: wifiIconName
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter

        horizontalAlignment: Text.AlignHCenter
        font.pointSize: 16
        text: qsTr("wifi设置")
    }
}
