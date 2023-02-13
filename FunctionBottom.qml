import QtQuick 2.0
import QtGraphicalEffects 1.0
//用于动态创建应用列表，以后实现
Item {
    width:170
    height: 200

    property string textApplicationName :"应用名"

    Rectangle{
        id:applicationLogo
        anchors.top: parent.top
        radius: 50
        width:170
        height: 170

        Image{
            id:applicationImage
            anchors.fill: parent
            source: "qrc:/img/img/wifi.jpg"
        }
    }
    Text {
        id: applicationName
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter

        horizontalAlignment: Text.AlignHCenter
        font.pointSize: 16
        text: qsTr(textApplicationName)   //应用名由C++发送来的模型决定
    }
    MouseArea {
        anchors.fill: parent
        onClicked: {
            //点击时通过数据列表来开启对应窗口
        }
    }
}
