import QtQuick 2.0
import QtQuick.Window 2.0
import QtQuick.Controls 2.0
import photoProject 1.0
//新建一个窗口，用来给光机投影图片

Window
{
    property alias imgPath: showImage.sourceImgPath
    flags:Qt.FramelessWindowHint
    height: 1080
    width: 1920
    x:0
    color: "black"

    Component.onCompleted:
    {
        console.log("投影窗口加载完成："+showImage.sourceImgPath);
//        showImage.fileUrl = "../projection/blackscreen.png"
    }

    PhotoProj
    {
        id:showImage
        property string sourceImgPath: "../projection/blackscreen.png"
        x:0
        height: 1080
        width:1920
        fileUrl: sourceImgPath
        scale: -1
    }
//    Image
//    {
//        objectName: "printImage"
//        id:printImage
//        property string sourceImgPath: ""
//        cache:false
//        anchors.fill: parent
//        source: sourceImgPath
//    }
//    Text {
//        objectName: "test"
//        id: test
//        color: "white"
//        anchors.left: parent.left;
//        anchors.leftMargin: 500;
//        anchors.top: parent.top;
//        anchors.topMargin: 500;
//        font.pixelSize: 26;
//    }

}

