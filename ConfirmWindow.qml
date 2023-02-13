import QtQuick 2.0

Rectangle {
    id:confirmBackground

    width:1920
    height:1080
    color:"#7F2F4F4F"
    signal closeConfirmWindow();
    signal clickOk(string type);
    property alias title: confirmBackground.windowTitle
    property alias content: confirmBackground.windowContent
    property alias cmdType: confirmBackground.type

    property string type:""
    property bool clickFlag: true;
    property string windowTitle: "test测试"
    property string windowContent: "test测试"
    MouseArea
    {
        anchors.fill: parent
        onClicked: confirmBackground.closeConfirmWindow();
    }
    Rectangle{
        id:confirmWindow
        width: 650
        height: 340
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.centerIn: parent.Center
        color: "white"
        Rectangle
        {
            id: confirmTitle
            height: 45
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            color: "#007aff"
            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pointSize: 20
                color:"white"
//                anchors.left: parent.left
//                anchors.right: parent.right

                text: qsTr(windowTitle)

            }
        }

        Rectangle
        {
            id: rectangle
            anchors.top: confirmTitle.bottom
            anchors.topMargin: 0
            anchors.horizontalCenter: parent.horizontalCenter
            width: 500
            height: 200
            Text {
                id: confirmContent
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pointSize: 18

                text: qsTr(windowContent)

            }
        }

        Rectangle
        {
            anchors.top:rectangle.bottom
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right

            Rectangle
            {
                id:okBottom
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 30
                anchors.left: parent.left
                anchors.leftMargin: 140
                border.color: "black"
                border.width: 1
                width: 100
                height: 50
                radius: 10

                Text {
                    text: qsTr("确定")
                    anchors.verticalCenter: parent.verticalCenter
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.pointSize: 15
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                MouseArea
                {
                    anchors.fill: parent
                    onClicked:
                    {
                        console.log("点击确定，type:",type)
                        confirmBackground.clickOk(type)
                    }

                }
            }
            Rectangle
            {
                id:cancelBottom
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 30
                anchors.right: parent.right
                anchors.rightMargin: 140
                border.color: "black"
                border.width: 1
                width: 100
                height: 50
                radius: 10
                MouseArea
                {
                    anchors.fill: parent
                    onClicked: confirmBackground.closeConfirmWindow()
                }
                Text {
                    text: qsTr("取消")
                    anchors.verticalCenter: parent.verticalCenter
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.pointSize: 15
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }
        }
    }
}
