import QtQuick 2.0
import qmlOpenDoor 1.0

Rectangle {
    width:134//180
    height: 180
    color: "#303248"
    Rectangle {
        width: 134
        height: 134
        radius: 67
        anchors.top: parent.top
        color: "#303248"
        Image {
            source: "qrc:/img/img/opendoor.png"
            anchors.centerIn: parent
        }
        MouseArea {
            anchors.fill: parent
            onClicked:
            {
                qmlopendoor.dooropenbutton_clicked()
            }
//            onPressAndHold:
//            {
//                applicationLoader.source = "AdjustPlaneWindow.qml"
//            }
        }
        QmlOpenDoor {
            id: qmlopendoor
        }

    }

    Text {
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        horizontalAlignment: Text.AlignHCenter
        font.pointSize: 18
        text: qsTr("开门")
        color: "white"
    }
}
