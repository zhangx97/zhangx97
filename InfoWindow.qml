import QtQuick 2.0

Rectangle//整个页面的半透明背景
{
    id:infoWindowBackground

    width:1920
    height:1080
    color:"#7F2F4F4F"

    signal closeInfoWindow()

    property alias title: infoWindowBackground.infoTitle
    property alias content: infoWindowBackground.infoContent

    property string infoTitle: ""
    property string infoContent: ""

    MouseArea//半透明可点击区域
    {
        anchors.fill: parent
        onClicked: infoWindowBackground.closeInfoWindow();
    }
    Rectangle//窗口主体
    {
        id:infoWindow
        width: 650
        height: 340
        anchors.horizontalCenter:parent.horizontalCenter
        anchors.verticalCenter:parent.verticalCenter

        Rectangle//标题
        {
            id:infoTitleRec
            height: 45
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            color: "#007aff"

            Text//标题内容
            {
                id: infoTitleText
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pointSize: 20
                color:"white"
                text: qsTr(infoWindowBackground.infoTitle)
            }
        }
        Rectangle//内容
        {
            anchors.top: infoTitleRec.bottom
            anchors.topMargin: 0
            anchors.horizontalCenter: parent.horizontalCenter
            width: 500
            height: 200

            Text {
                id: infoContentText

                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pointSize: 18
                text: qsTr(infoWindowBackground.infoContent)
            }
        }
        Rectangle//确定按钮
        {
            border.color: "black"
            border.width: 1
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 30
            anchors.horizontalCenter: parent.horizontalCenter
            width: 100
            height: 50
            radius: 10

            Text {
                id: infoOkText
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pointSize: 15
                text: qsTr("确定")
            }
            MouseArea
            {
                anchors.fill: parent
                onClicked: infoWindowBackground.closeInfoWindow();
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.66}D{i:1}D{i:3}D{i:5}D{i:7}D{i:2}
}
##^##*/
