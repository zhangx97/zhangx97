import QtQuick 2.0
import QtGraphicalEffects 1.0

Rectangle {
    width: 215
    height: 1080
    color: "#303248"

    signal slicepackReload(string modelname)

//    Rectangle {
//        id: logo
//        width: 180
//        height: 100
//        anchors.top: parent.top
//        anchors.topMargin: 70
//        anchors.horizontalCenter: parent.horizontalCenter

//        Image {
//            id: logoimage
//            source: "qrc:/img/img/筑真logo.png"
//            anchors.centerIn: parent
//        }

//        ColorOverlay {
//            anchors.fill: logoimage
//            source: logoimage
//            color: "white"
//        }
//    }

    Image {
        id: logo
        source: "qrc:/img/img/筑真logo.png"
        anchors.top: parent.top
        anchors.topMargin: 70
        anchors.horizontalCenter: parent.horizontalCenter

        MouseArea {
            anchors.fill: parent
            onPressAndHold: {
                applicationLoader.source = "AdjustPlaneWindow.qml"
            }
        }
    }

    ColorOverlay {
        anchors.fill: logo
        source: logo
        color: "white"
    }

    Rectangle {
        id: zzdbrec
        width: 215
        height: 60
        anchors.top: parent.top
        anchors.topMargin: 240
        anchors.left: parent.left
        focus: true
        color: "#303248"

        Image {
            id: zzdbimage
            source: zzdbrec.focus ? "qrc:/img/img/model.png" : ""
            anchors.centerIn: parent
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                zzdbrec.focus = true
                console.log("种植导板onClicked")
                slicepackReload("ZZDB*.zip")
            }
        }

        Text {
            id: zzdb
            text: qsTr("种植导板")
            color: "white"
            font.pointSize: 24
            anchors.centerIn: parent

        }
    }
    Rectangle {
        id: ksmxrec
        width: 215
        height: 60
        anchors.top: zzdbrec.bottom
        anchors.topMargin: 20
        anchors.left: parent.left
        focus: false
        color: "#303248"
        Image {
            id: ksmximage
            source: ksmxrec.focus ? "qrc:/img/img/model.png" : ""
            anchors.centerIn: parent
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                ksmxrec.focus = true
                console.log("口扫模型onClicked")
                slicepackReload("*MX*.zip")
            }
        }

        Text {
            id: ksmx
            text: qsTr("口扫模型")
            color: "white"
            font.pointSize: 24
            anchors.centerIn: parent

        }
    }
//    Rectangle {
//        id: lsxfrec
//        width: 215
//        height: 60
//        anchors.top: ksmxrec.bottom
//        anchors.topMargin: 20
//        anchors.left: parent.left
//        focus: false
//        color: "#303248"
//        Image {
//            id: lsxfimage
//            source: lsxfrec.focus ? "qrc:/img/img/model.png" : ""
//            anchors.centerIn: parent
//        }
//        MouseArea {
//            anchors.fill: parent
//            onClicked: {
//                lsxfrec.focus = true
//                console.log("临时修复onClicked")
//                slicepackReload("*.zip")
//            }
//        }

//        Text {
//            id: lsxf
//            text: qsTr("临时修复")
//            color: "white"
//            font.pointSize: 24
//            anchors.centerIn: parent

//        }
//    }
//    Rectangle {
//        id: qtyyrec
//        width: 215
//        height: 60
//        anchors.top: lsxfrec.bottom
//        anchors.topMargin: 20
//        anchors.left: parent.left
//        focus: false
//        color: "#303248"
//        Image {
//            id: qtyyimage
//            source: qtyyrec.focus ? "qrc:/img/img/model.png" : ""
//            anchors.centerIn: parent
//        }

//        MouseArea {
//            anchors.fill: parent
//            onClicked: {
//                qtyyrec.focus = true
//                console.log("其他应用onClicked")
//                slicepackReload("*.zip")
//            }
//        }

//        onActiveFocusChanged: {
//            console.log("active focus changed", activeFocus)
//        }

//        Text {
//            id: qtyy
//            text: qsTr("其他应用")
//            color: "white"
//            font.pointSize: 24
//            anchors.centerIn: parent

//        }
//    }
}
