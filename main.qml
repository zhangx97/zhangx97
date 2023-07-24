import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.VirtualKeyboard 2.4
import QtQuick.VirtualKeyboard.Settings 2.2
import QtQuick.Controls 2.5
import QtQml.Models 2.3
import QtGraphicalEffects 1.0
import qmlShowSlicePack 1.0
import qmlReadModelPack 1.0
//import qmlEth0Running 1.0
//import qmlPrinting 1.0
import qmlControl.item 1.0
import adjustPlaneVoltage 1.0
import qmlState.item 1.0
import qmlUdiskTransfer 1.0



Window {
    id: window
    visible: true
    width: 1920
    height: 1080
    x: 1920
    flags: Qt.FramelessWindowHint
    color: "#f2f2f7"

    property string slicepackName: ""
    property string material: ""
    property string count: ""
    property string thick: ""
    property string weight: ""
    property string duration: ""

    property int statusCurrent: 2
    property int statusLight: 70
    property int statusResinTemp: 25
    property int statusTemp: 40

    signal startPrinting(string slicePackPath)

    Component.onCompleted:
    {
//        var component = Qt.createComponent("ProjectionArea.qml");
//        var win = component.createObject(null);
//        win.show();
//        win.closing.connect(function() { console.log('窗口关闭了') })
//        printImagePage.imgPath = "../projection/blackscreen.png"
        printImagePage.show();
    }

    ProjectionArea//投影窗口
    {
        objectName: "printImagePage";
        id:printImagePage
        imgPath: "../projection/blackscreen.png"
    }

    AdjustPlane {
        id: adjustPlane
    }

    Image {
        id: viewImage
        source: "qrc:/img/img/icon2-07.png"
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.right: parent.right
        anchors.rightMargin: 40
        ColorOverlay {
            anchors.fill: viewImage
            source: viewImage
            color: "grey"
        }
    }

    QmlUdiskTransfer {
        id:udisktransfer
    }

    Image {
        id: usbImage
        source: "qrc:/img/img/icon2-03.png"
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.right: viewImage.left
        anchors.rightMargin: 40
        ColorOverlay {
            anchors.fill: usbImage
            source: usbImage
            color: "grey"
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                remindWindow.source = "ConfirmWindow.qml"
                remindWindow.item.title = "U盘传输文件"
                remindWindow.item.content = "是否将U盘中的slicepack文件复制到设备中？"
                remindWindow.item.cmdType = "udisk"
            }
        }
    }

    //    QmlEth0Running {
    //        id: eth0running
    //    }

    Image {
        id: lanImage
        source: "qrc:/img/img/icon2-04.png"
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.right: usbImage.left
        anchors.rightMargin: 40
        ColorOverlay {
            anchors.fill: lanImage
            source: lanImage
//            color: eth0running.returnEth0Running() ? "#20468a" : "grey"
            color: "grey"
        }

        Component.onCompleted: {
//            console.log("eth0running return: " + eth0running.returnEth0Running())
        }
    }

    Loader {
        z: 1
        id:applicationLoader
        anchors.fill:parent
    }


    Image {
        id: wifiImage
        source: "qrc:/img/img/icon2-01.png"
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.right: lanImage.left
        anchors.rightMargin: 40
        ColorOverlay {
            anchors.fill: wifiImage
            source: wifiImage
            color: "#20468a"
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                applicationLoader.source = "qrc:/wifi/WifiPage.qml"
            }
        }
    }

    Image {
        id: setImage
        source: "qrc:/img/img/icon2-05.png"
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.right: wifiImage.left
        anchors.rightMargin: 40
        ColorOverlay {
            anchors.fill: setImage
            source: setImage
            color: "grey"
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                applicationLoader.source = "qrc:/setting/SettingPage.qml"
            }
        }
    }

    MainMenuBar {
        id: mainmenubar
        x: 0
        y: 0
    }

    Rectangle {
        id: slicepack
        height: 360
        width: 1270
        color: "white"
        clip: true

        anchors.left: mainmenubar.right
        anchors.leftMargin: 20
        anchors.top: dateRec.bottom
        anchors.topMargin: 5


        QmlShowSlicePack {
            id: qmlshowslicepack
        }

        ListView {
            id: slicepacklistview
            x: 8
            y: 8
            height: 352
            width: 1254
            model: qmlshowslicepack
            delegate: slicepackdelegate
            spacing: 8
            focus: true

            snapMode: ListView.NoSnap//SnapOneItem
            orientation: ListView.Horizontal

            ScrollBar.horizontal: ScrollBar {
                policy: ScrollBar.AlwaysOn
            }
        }
        Component {
            id: slicepackdelegate
            Rectangle {
                id: slicepackitem
                height: 340
                width: 412
                property string firstSlicepackName: FirstName
                Rectangle {
                    id: slicepackrec1
                    height: 166
                    width: 412
                    border.width: 2
                    property bool firstIsCurrentItem: true
                    border.color: firstIsCurrentItem  & slicepackitem.ListView.isCurrentItem ? "#20468a":"grey"
                    Text {
                        id: slicepacktext1
                        text: FirstNum + ".  " + FirstName
                        font.pixelSize: 28
                        color: slicepackrec1.firstIsCurrentItem  & slicepackitem.ListView.isCurrentItem ? "#20468a":"black"
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.verticalCenterOffset: -25
                    }
                    Text {
                        id: infoText1
                        text: "共" + FirstSize + "MB" + " | " + FirstDate + " | 本地文件"
                        font.pixelSize: 16
                        color: slicepackrec1.firstIsCurrentItem  & slicepackitem.ListView.isCurrentItem ? "#20468a":"grey"
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.verticalCenterOffset: 25
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            slicepacklistview.currentIndex = index
                            slicepackrec1.firstIsCurrentItem = true
                            slicepackrec2.secondIsCurrentItem = false
                            console.log(slicepackName)
                            slicepackName = FirstName
                            console.log(slicepackName)
                            readModelPack.getModelPackInof(slicepackName)
                            material = readModelPack.material
                            count = readModelPack.count
                            thick = readModelPack.thick
                            weight = readModelPack.weight
                            duration = readModelPack.duration
                        }
                    }
                }
                Rectangle {
                    id: slicepackrec2
                    height: 166
                    width: 412
                    border.width: 2
                    property bool secondIsCurrentItem: false
                    border.color: secondIsCurrentItem & slicepackitem.ListView.isCurrentItem ? "#20468a":"grey"
                    anchors.left: slicepackrec1.left
                    anchors.top: slicepackrec1.bottom
                    anchors.topMargin: 8
                    Text {
                        id: slicepacktext2
                        text: SecondNum + ".  " + SecondName
                        font.pixelSize: 28
                        color: slicepackrec2.secondIsCurrentItem & slicepackitem.ListView.isCurrentItem ? "#20468a":"black"
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.verticalCenterOffset: -25
                    }
                    Text {
                        id: infoText2
                        text: "共" + SecondSize + "MB" + " | " + SecondDate + " | 本地文件"
                        font.pixelSize: 16
                        color: slicepackrec2.secondIsCurrentItem & slicepackitem.ListView.isCurrentItem ? "#20468a":"grey"
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.verticalCenterOffset: 25
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            if (SecondName != "more") {
                                slicepacklistview.currentIndex = index
                                slicepackrec1.firstIsCurrentItem = false
                                slicepackrec2.secondIsCurrentItem = true
                                console.log(slicepackName)
                                slicepackName = SecondName
                                console.log(slicepackName)
                                readModelPack.getModelPackInof(slicepackName)
                                material = readModelPack.material
                                count = readModelPack.count
                                thick = readModelPack.thick
                                weight = readModelPack.weight
                                duration = readModelPack.duration
                            }
                        }
                    }
                }
                Component.onCompleted: {
//                    console.log(slicepacklistview.contentItem.children[0].children[1].children[0].text)
                    slicepackName = slicepacklistview.contentItem.children[0].firstSlicepackName
                    console.log("test")
                    console.log(slicepackName)
                }
            }
        }

        Connections {
            target: mainmenubar
            function onSlicepackReload(modelname) {
                qmlshowslicepack.reload(modelname)
            }
        }
    }

    OpenDoorButton {
        id: openDoorButton
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 70
        anchors.left: parent.left
        anchors.leftMargin: 40
    }

    Rectangle {
        id: dateRec
        anchors.left: mainmenubar.right
        anchors.leftMargin: 20
        anchors.top: parent.top
        anchors.topMargin: 80
        anchors.right: parent.right
        anchors.rightMargin: 20
        height: 100
        width: 1665

        Image {
            id: dateRecimage
            source: "qrc:/img/img/icon2-09.png"
            anchors.left: parent.left
            anchors.leftMargin: 25
            anchors.verticalCenter: parent.verticalCenter
        }

        ColorOverlay {
            anchors.fill: dateRecimage
            source: dateRecimage
            color: "#5d7deb"
        }

        Text {
            id: textDateTime
            text: currentDateTime()
            anchors.left: dateRecimage.right
            anchors.leftMargin: 15
            anchors.verticalCenter: parent.verticalCenter
            color: "#5d7deb"
            font.pixelSize: 22
        }

        Timer {
            id: dateTimer
            interval: 1     //间隔(单位毫秒):1000毫秒=1秒
            repeat: true    //重复
            onTriggered: {
                textDateTime.text = currentDateTime()
            }
        }

        Component.onCompleted: {
            dateTimer.start()
        }
    }

    function currentDateTime() {
        return Qt.formatDateTime(new Date(),"dddd/yyyy.MM.dd      HH:mm:ss")
    }

    Rectangle {
        id: status
        height: 360
        width: 390
        anchors.left: slicepack.right
        anchors.leftMargin: 5
        anchors.right: parent.right
        anchors.rightMargin: 20
        anchors.top: dateRec.bottom
        anchors.topMargin: 5

        Grid {
            id:panelGrid
            columns: 2
            columnSpacing: 30
            rowSpacing: 10

            anchors.top: parent.top
            anchors.topMargin: 15
            anchors.left: parent.left
            anchors.leftMargin: 20

            Repeater {
                model: [
                    {"name":"光机DAC","color":"#34c759","value":window.statusCurrent,"maxValue":1024,"unit":""},
                    {"name":"光机亮度","color":"#007aff","value":window.statusLight,"maxValue":100,"unit":""},
                    {"name":"光机温度","color":"#ff3b30","value":window.statusTemp,"maxValue":120,"unit":"­°C"},
                    {"name":"材料温度","color":"#F83E05","value":window.statusResinTemp,"maxValue":100,"unit":"­°C"}
                ]
                Rectangle {
                    width: 160
                    height: 160
                    MachineStatusPanel {
                        propertyName:modelData.name
                        propertyNumberValue: modelData.value
                        panelColor:modelData.color
                        maxValue:modelData.maxValue
                        size:160
                        unit:modelData.unit
                    }
                }
            }
        }

    }

    Rectangle {
        id: photo
        width: 1070
        height: 515
        anchors.left: mainmenubar.right
        anchors.leftMargin: 20
        anchors.top: slicepack.bottom
        anchors.topMargin: 5
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 15

        Text {
            id: photoOneText
            text: slicepackName
            font.pixelSize: 24
            anchors.left: parent.left
            anchors.leftMargin: 75
            anchors.top: parent.top
            anchors.topMargin: 15
        }

//        Text {
//            id: photoTwoText
//            text: "包含1个模型"
//            font.pixelSize: 16
//            anchors.left: photoOneText.right
//            anchors.leftMargin: 40
//            anchors.bottom: photoOneText.bottom
//            color: "grey"
//        }

        Rectangle {
            id:photorec
            width: 768
            height: 432
//            color: "blue"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 25

            SwipeView {
                id: photoswi
                currentIndex: 1
                anchors.fill: parent			//SwipeView的弊端只能依赖于anchors.fill: parent
                clip: true                      //超出父控件的部分不显示
                Item {
                    id: firstPage
//                    Rectangle {
//                        anchors.fill: parent
//                        color: "lightsteelblue"
//                    }
                    Image {
                        id: firstImage
                        source: readModelPack.getPng(slicepackName, "BottomView.png")
                        anchors.fill: parent
                    }
                }
                Item {
                    id: secondPage
//                    Rectangle {
//                        anchors.fill: parent
//                        color: "grey"
//                    }
                    Image {
                        id: secondImage
                        source: readModelPack.getPng(slicepackName, "Preview.png")
                        anchors.fill: parent
                    }
                }
                Item {
                    id: thirdPage
//                    Rectangle {
//                        anchors.fill: parent
//                        color: "black"
//                    }
                    Image {
                        id: thirdImage
                        source: readModelPack.getPng(slicepackName, "BeautifulView.png")
                        anchors.fill: parent
                    }
                }
            }

            PageIndicator {
                id: indicator

                count: photoswi.count
                currentIndex: photoswi.currentIndex

                anchors.bottom: photoswi.bottom
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
    }

    QmlReadModelPack {
        id: readModelPack
        slicepackPath: slicepackName
        Component.onCompleted: {
//            console.log("1234 - " + readModelPack.slicepackPath)
//            readModelPack.qDebugNowSlicepackPathChanged()
            console.log("readModelPack 1")
            console.log(readModelPack.material)
            console.log(readModelPack.count)
            console.log(readModelPack.thick)
            console.log(readModelPack.weight)
            console.log(readModelPack.duration)
            readModelPack.getModelPackInof(slicepackName)
            console.log("readModelPack 2")
            console.log(readModelPack.material)
            console.log(readModelPack.count)
            console.log(readModelPack.thick)
            console.log(readModelPack.weight)
            console.log(readModelPack.duration)
            readModelPack.count = "789"
            console.log(readModelPack.count)
            material = readModelPack.material
            count = readModelPack.count
            thick = readModelPack.thick
            weight = readModelPack.weight
            duration = readModelPack.duration
//            materialData.text = readModelPack.material
//            countData.text = readModelPack.count
//            thickData.text = readModelPack.thick
//            weight.text = readModelPack.weight
//            durationData.text = readModelPack.duration
        }
    }

    Rectangle {
        id: startprint
        height: 515
        width: 590
        anchors.left: photo.right
        anchors.leftMargin: 5
        anchors.right: parent.right
        anchors.rightMargin: 20
        anchors.top: status.bottom
        anchors.topMargin: 5
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 15


        Canvas {
            z: 1
            anchors.fill: parent
            onPaint: {
                var ctx = getContext("2d")
                ctx.lineWidth = 1
                ctx.strokeStyle = "grey"
//                ctx.fillStyle = "steelblue"
                ctx.beginPath()
                ctx.moveTo(25, 115)
                ctx.lineTo(565, 115)
                ctx.stroke()

                ctx.beginPath()
                ctx.moveTo(25, 205)
                ctx.lineTo(565, 205)
                ctx.stroke()

                ctx.beginPath()
                ctx.moveTo(295, 123)
                ctx.lineTo(295, 197)
                ctx.stroke()

                ctx.beginPath()
                ctx.moveTo(295, 213)
                ctx.lineTo(295, 287)
                ctx.stroke()
            }
        }

        Rectangle {
            id: materialrec
            width: 590
            height: 115

            Text {
                id: materialTitle
                text: qsTr("材料型号")
                font.pixelSize: 24
                y: materialrec.height/7*2
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Text {
                id: materialData
                text: material
                font.pixelSize: 20
                y: materialrec.height/7*4
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }

        Rectangle {
            id: countrec
            width: 295
            height: 90
            anchors.left: parent.left
            anchors.top: materialrec.bottom
            Text {
                id: countTitle
                text: qsTr("切层数")
                font.pixelSize: 24
                y: countrec.height/7
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Text {
                id: countData
                text: count + "层"
                font.pixelSize: 20
                y: countrec.height/7*4
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Component.onCompleted: {
                console.log("countrec")
                console.log(readModelPack.count)
            }
        }
        Rectangle {
            id: thickrec
            width: 295
            height: 90
            anchors.right: parent.right
            anchors.top: materialrec.bottom
            Text {
                id: thickTitle
                text: qsTr("打印层厚")
                font.pixelSize: 24
                y: thickrec.height/7
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Text {
                id: thickData
                text: thick+"mm"
                font.pixelSize: 20
                y: thickrec.height/7*4
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
        Rectangle {
            id: weightrec
            width: 295
            height: 90
            anchors.left: parent.left
            anchors.top: countrec.bottom
            Text {
                id: weightTitle
                text: qsTr("预估重量")
                font.pixelSize: 24
                y: weightrec.height/7
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Text {
                id: weightData
                text: weight + "g"
                font.pixelSize: 20
                y: weightrec.height/7*4
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
        Rectangle {
            id: durationrec
            width: 295
            height: 90
            anchors.right: parent.right
            anchors.top: thickrec.bottom
            Text {
                id: durationTitle
                text: qsTr("预估时长")
                font.pixelSize: 24
                y: durationrec.height/7
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Text {
                id: durationData
                text: duration
                font.pixelSize: 20
                y: durationrec.height/7*4
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }

        Rectangle {
            width: 540
            height: 100
            color: "#1664fc"
            radius: 5
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 56
            anchors.horizontalCenter: parent.horizontalCenter

            Text {
                text: qsTr("开始打印")
                font.pixelSize: 40
                color: "white"
                anchors.centerIn: parent
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    startPrinting(slicepackName)
                }
            }
        }


////        Component.onCompleted: {
////            readModelPack.getModelPackInof(slicepackName)
////            materialData.text = readModelPack.material
////            countData.text = readModelPack.count
////            thickData.text = readModelPack.thick
////            weight.text = readModelPack.weight
////            durationData.text = readModelPack.duration
////        }
    }

//    QmlPrinting {
//        id: qmlprinting
//    }

    QMLControl//qml打印控制类
    {
        id:qmlControl
    }

    //连接qml内的打印控制类
    Connections
    {
        target:qmlControl
        ignoreUnknownSignals: true
        function onChangePhoto(path)
        {
            console.log("onChangePhoto");
            printImagePage.imgPath = path;
        }
        function onUnzipFinish()
        {
            //loader打开另一个窗口
            applicationLoader.source = "qrc:/printControl/PrintControlWindow.qml"
        }
        function onConfirmUpdate()
        {
            //弹窗确认是否要更新，还没做好
//            console.log("到达弹窗确认这步");
            remindWindow.source = "ConfirmWindow.qml"
            remindWindow.item.title = "更新"
            remindWindow.item.content = "新的版本已经准备完毕,是否立即开始更新？"
            remindWindow.item.cmdType = "upgrade"
        }
        function onPrintComplete()
        {
            console.log("正常完成打印发送的信号");
            applicationLoader.source="";
            remindWindow.source ="qrc:/InfoWindow.qml"
            remindWindow.item.title = "打印结束"
            remindWindow.item.content = "已成功完成打印,请等待平台上升到顶..."
            console.log("打印详情页退出");
        }
        function onStopPrint()
        {
            console.log("qml界面收到停止打印信号");
            remindWindow.source = "ConfirmWindow.qml"
            remindWindow.item.title = "提醒"
            remindWindow.item.content = "打印结束！！！"
            remindWindow.item.cmdType = "destroy"
        }
    }

    Loader//加载确认框
    {
        id:remindWindow
        anchors.fill: parent
    }

    Connections {
        target: window
        ignoreUnknownSignals: true
        function onStartPrinting(slicepackName) {
            console.log("qml信号中传递的路径：",slicepackName)
//            slicePath = slicePackPath
            remindWindow.source = "ConfirmWindow.qml"
            remindWindow.item.title = "确认开始打印"
            remindWindow.item.content = "是否确认开始打印？"
            remindWindow.item.cmdType = "print"
        }
    }

    Connections {
        target: udisktransfer
        ignoreUnknownSignals: true
        function onNodevsda1() {
            remindWindow.source = "ConfirmWindow.qml"
            remindWindow.item.title = "未识别到U盘"
            remindWindow.item.content = "请检查U盘？"
            remindWindow.item.cmdType = "destroy"
        }
        function onNoudisksetting() {
            remindWindow.source = "ConfirmWindow.qml"
            remindWindow.item.title = "U盘配置文件错误"
            remindWindow.item.content = "请检查U盘配置文件？"
            remindWindow.item.cmdType = "destroy"
        }
        function onUdiskTransferSuccess(files, filescount) {
            remindWindow.source = "ConfirmWindow.qml"
            remindWindow.item.title = "文件传输完成"
            remindWindow.item.content = `已成功将${filescount}个文件传输到设备中！`
            remindWindow.item.cmdType = "destroy"
        }
    }

    //连接的是applicationLoader加载对象的信号
    Connections
    {
        target:applicationLoader.item
        ignoreUnknownSignals: true
        function onCloseWifiWindow()
        {
            applicationLoader.source = "";
        }
        function onCloseSettingWindow()
        {
            applicationLoader.source = "";
        }
        function onClosePrintWindow()
        {
            applicationLoader.source="";
            remindWindow.source ="qrc:/InfoWindow.qml"
            //remindWindow.item.cmdType = "stopPrint"
            remindWindow.item.title = "停止打印"
            remindWindow.item.content = "正在停止打印,请等待平台上升到顶..."
            console.log("打印详情页退出");
        }
        function onStartUpgrade(version)
        {
            console.log("向后台发送需要下载的版本"+version);
            qmlControl.checkUpdate(version);
        }
        function onProtectScreenTime(time)
        {
            console.log("向后台传输屏保时间"+time);
            screenSaver.setScreenProtectTime(time);
        }
        function onHeatStatusChange(flag)
        {
            qmlControl.heatSwitch(flag);
        }
        function onCloseLogWindow()
        {
            applicationLoader.source = "";
        }
        function onSendCmd(cmd)
        {
            console.log("收到调平窗口的信号："+cmd);
            adjustPlane.sendCmd(cmd);
        }
    }

    Connections {
        target: remindWindow.item
        ignoreUnknownSignals: true
        function onClickOk(type)
        {
            if(type === "print")
            {
                remindWindow.source = "";
                qmlControl.printStart(slicepackName)
                console.log("主程序页面clickok信号触发");
            }
            if(type === "udisk")
            {
                remindWindow.source = "ConfirmWindow.qml"
                remindWindow.item.title = "文件传输中"
                remindWindow.item.content = "请等待文件传输完毕......"
                remindWindow.item.cmdType = "destroy"
                udisktransfer.udiskTranserButton_clicked();
                console.log("udisk");
            }
            if(type === "destroy")
            {
                remindWindow.source = "";
                console.log("destroy");
            }
        }
        function onCloseInfoWindow()
        {
            remindWindow.source = "";
            console.log("onCloseInfoWindow触发");
        }
        function onCloseConfirmWindow()
        {
            remindWindow.source = "";
            console.log("主程序页面关闭提示窗信号触发");
        }
    }

    Connections//调节水平
    {
        target:adjustPlane
        ignoreUnknownSignals: true
//        function onVoltageChange(voltage)
//        {
//            if(voltage)
//            {
//                applicationLoader.source = "qrc:/AdjustPlaneWindow.qml";
//                console.log("显示调整平衡窗口");
//            }
//            else
//            {
//                applicationLoader.source = "";
//                console.log("关闭调整平衡窗口");
//            }
//        }
        function onPositionChange(str)
        {
//            adjustPlane.showNewPosition(str).connect(applicationLoader.item.newPosition(str));
            applicationLoader.item.newPosition(str);
        }
    }

    QMLState
    {
        id:printStateqml
    }

    Connections//连接qml状态类
    {
        target: printStateqml
        ignoreUnknownSignals: true
//        function onPrintStateChanged(nowPrintState)
//        {
//            if(nowPrintState === "start" || nowPrintState === "continue")
//            {
//                machineStatus.machineStatusText = "开始打印"
//            }
//            else if(nowPrintState === "stop")
//            {
//                machineStatus.machineStatusText =  "正在停止"
//            }
//            else if(nowPrintState === "heat")
//            {
//                machineStatus.machineStatusText =  "加热中"
//            }
//            else if(nowPrintState === "pause")
//            {
//                machineStatus.machineStatusText =  "暂停"
//            }
//            else if(nowPrintState === "NULL")
//            {
//                machineStatus.machineStatusText =  "空闲"
//            }
//        }
//        function onPrintIDChange(printID)
//        {
//            machineStatus.serialNumber = printID;
//        }
        function onLedTempChanged(temperature)
        {
            //console.log("更新圆环状态");
            window.statusLight = printStateqml.ledBrightness;
            window.statusCurrent = printStateqml.ledCurrent;
            window.statusResinTemp = printStateqml.resinTemperature;
            window.statusTemp = temperature;
        }
    }

    InputPanel {
        id: inputPanel
        z: 99
        x: 0
        y: window.height
        width: window.width
        Component.onCompleted:
        {
            VirtualKeyboardSettings.activeLocales = ["en_GB","zh_CN"]
            VirtualKeyboardSettings.locale = "en_GB"
        }

        states: State {
            name: "visible"
            when: inputPanel.active
            PropertyChanges {
                target: inputPanel
                y: window.height - inputPanel.height
            }
        }
        transitions: Transition {
            from: ""
            to: "visible"
            reversible: true
            ParallelAnimation {
                NumberAnimation {
                    properties: "y"
                    duration: 250
                    easing.type: Easing.InOutQuad
                }
            }
        }
    }
}


//import QtQuick 2.11
//import QtQuick.Window 2.11
//import QtQuick.Controls 2.4
//import QtQuick.VirtualKeyboard 2.2
//import QtQuick.VirtualKeyboard.Settings 2.2
//import qmlControl.item 1.0
//import qmlState.item 1.0
//import adjustPlaneVoltage 1.0
//import screenProtect 1.0
//import QtGraphicalEffects 1.0

//ApplicationWindow {
//    id: applicationWindow
//    objectName: "rootObject"
//    visible: true
//    x:1920
//    width: 1920
//    height: 1080
//    flags:Qt.FramelessWindowHint
//    color: "#f2f2f7"
//    property string slicePath: ""//需要被打印的slicepack名称

//    Component.onCompleted:
//    {
////        var component = Qt.createComponent("ProjectionArea.qml");
////        var win = component.createObject(null);
////        win.show();
////        win.closing.connect(function() { console.log('窗口关闭了') })
////        printImagePage.imgPath = "../projection/blackscreen.png"
//        printImagePage.show();
//    }

////    AdjustPlane
////    {
////        id:adjustPlane
//////        signal showNewPosition(string position);
////    }

//    ProjectionArea//投影窗口
//    {
//        objectName: "printImagePage";
//        id:printImagePage
//        imgPath: "../projection/blackscreen.png"
//    }

//    StatusArea{//状态页面
//        id:machineStatus
//        anchors.left: parent.left
//        anchors.leftMargin: 50
//        anchors.top: parent.top
//        anchors.topMargin: 200
//        machineStatusText:"空闲"
//        width: 640
//        height: 500
//    }

//    SlicePackArea{//切层图包和开始打印
//        id:printArea
//        anchors.left: machineStatus.right
//        anchors.leftMargin: 10
//        height: 500
//        anchors.top: parent.top
//        anchors.topMargin: 200
//        anchors.right: parent.right
//        anchors.rightMargin: 50
//    }

//    //wifi/设置这些按钮以后可以通过repeater来加载
//    WifiFunctionBottom//wifi页面
//    {
//        id:wifiBottom
//        anchors.left: parent.left
//        anchors.leftMargin: 50
//        anchors.bottom: parent.bottom
//        anchors.bottomMargin: 50
//        width:170
//        height: 240
//        MouseArea
//        {
//            anchors.fill: parent
//            onClicked:
//            {
//                applicationLoader.source = "qrc:/wifi/WifiPage.qml";
//            }
//        }
//    }

//    SettingBottom//设置页面
//    {
//        id:settingBottom
//        anchors.left: wifiBottom.right
//        anchors.leftMargin: 50
//        anchors.bottom: parent.bottom
//        anchors.bottomMargin: 50
//        width:170
//        height: 240
//        MouseArea
//        {
//            anchors.fill: parent
//            onClicked:
//            {
//                applicationLoader.source = "qrc:/setting/SettingPage.qml";
//            }
//        }
//    }

//    LogBottom
//    {
//        id:logBottom
//        anchors.left: settingBottom.right
//        anchors.leftMargin: 50
//        anchors.bottom: parent.bottom
//        anchors.bottomMargin: 50
//        width: 170
//        height: 240
//        MouseArea
//        {
//            anchors.fill: parent
//            onClicked:
//            {
//                applicationLoader.source = "qrc:/log/ShowLogPage.qml";
//            }
//        }
//    }

//    Loader//加载应用界面
//    {
//        id:applicationLoader
//        anchors.fill:parent
//    }

//    Loader//加载确认框
//    {
//        id:remindWindow
//        anchors.fill: parent
//    }

////    Loader//加载锁屏界面
////    {
////        id:screenLock
////        anchors.fill: parent
////    }

//    QMLControl//qml打印控制类
//    {
//        id:qmlControl
//    }

//    QMLState
//    {
//        id:printStateqml
//    }

//    ScreenSaver
//    {
//        id:screenSaver
//    }
//    //连接后台检查鼠标活动的类
//    Connections
//    {
//        target: screenSaver
//        function onStartBlack()
//        {
//            screenLock.source = "qrc:/screenProtect/ScreenProtect.qml"
//        }
//    }
//    //连接屏幕保护的loader
//    Connections
//    {
//        target: screenLock.item
//        function onCancelScreen()
//        {
//            screenLock.source = "";
//        }
//    }

//    //连接的是applicationLoader加载对象的信号
//    Connections
//    {
//        target:applicationLoader.item
//        ignoreUnknownSignals: true
//        function onCloseWifiWindow()
//        {
//            applicationLoader.source = "";
//        }
//        function onCloseSettingWindow()
//        {
//            applicationLoader.source = "";
//        }
//        function onClosePrintWindow()
//        {
//            applicationLoader.source="";
//            console.log("打印详情页退出");
//        }
//        function onStartUpgrade(version)
//        {
//            console.log("向后台发送需要下载的版本"+version);
//            qmlControl.checkUpdate(version);
//        }
//        function onProtectScreenTime(time)
//        {
//            console.log("向后台传输屏保时间"+time);
//            screenSaver.setScreenProtectTime(time);
//        }
//        function onHeatStatusChange(flag)
//        {
//            qmlControl.heatSwitch(flag);
//        }
//        function onCloseLogWindow()
//        {
//            applicationLoader.source = "";
//        }
//        function onSendCmd(cmd)
//        {
//            console.log("收到调平窗口的信号："+cmd);
//            adjustPlane.sendCmd(cmd);
//        }
//    }
//    //连接提示框发出的信号
//    Connections
//    {
//        ignoreUnknownSignals: true
//        target:remindWindow.item
//        //弹窗点击确认后，接受到的指令与对应的操作
//        //指令都是小写开头，打印指令为print
//        function onClickOk(type)
//        {
//            if(type === "print")
//            {
//                remindWindow.source = "";
//                //发送开始打印指令
//                qmlControl.printStart(slicePath)
//                console.log("主程序页面clickok信号触发");
//            }
//            if(type === "upgrade")
//            {
//                remindWindow.source = "";
//                qmlControl.startUpgrade();
//            }
//        }
//        function onCloseConfirmWindow()
//        {
//            remindWindow.source = "";
//            console.log("主程序页面关闭提示窗信号触发");
//        }
//        function onStartInstall()
//        {
//            qmlControl.zipReadySlot();
//        }
//    }

//    //显示弹窗
//    Connections
//    {
//        target: printArea
//        ignoreUnknownSignals: true
//        function onStartPrint(slicePackPath){
//            console.log("qml信号中传递的路径：",slicePackPath)
//            slicePath = slicePackPath
//            remindWindow.source = "ConfirmWindow.qml"
//            remindWindow.item.title = "确认开始打印"
//            remindWindow.item.content = "是否确认开始打印？"
//            remindWindow.item.cmdType = "print"
//        }
//        function onChangePackInfo(array)
//        {
//            //这里接收数组,显示到状态框里
//            machineStatus.slicepackInfo = array;
//        }
//    }

//    //连接qml内的打印控制类
//    Connections
//    {
//        target:qmlControl
//        ignoreUnknownSignals: true
//        function onChangePhoto(path)
//        {
//            printImagePage.imgPath = path;
//        }
//        function onUnzipFinish()
//        {
//            //loader打开另一个窗口
//            applicationLoader.source = "qrc:/printControl/PrintControlWindow.qml"
//        }
//        function onConfirmUpdate()
//        {
//            //弹窗确认是否要更新，还没做好
////            console.log("到达弹窗确认这步");
//            remindWindow.source = "ConfirmWindow.qml"
//            remindWindow.item.title = "更新"
//            remindWindow.item.content = "新的版本已经准备完毕,是否立即开始更新？"
//            remindWindow.item.cmdType = "upgrade"
//        }
//        function onStopPrint()
//        {
//            console.log("qml界面收到停止打印信号");
//            applicationLoader.item.printFinish();
//        }
//    }

//    Connections//连接qml状态类
//    {
//        target: printStateqml
//        ignoreUnknownSignals: true
//        function onPrintStateChanged(nowPrintState)
//        {
//            if(nowPrintState === "start" || nowPrintState === "continue")
//            {
//                machineStatus.machineStatusText = "开始打印"
//            }
//            else if(nowPrintState === "stop")
//            {
//                machineStatus.machineStatusText =  "正在停止"
//            }
//            else if(nowPrintState === "heat")
//            {
//                machineStatus.machineStatusText =  "加热中"
//            }
//            else if(nowPrintState === "pause")
//            {
//                machineStatus.machineStatusText =  "暂停"
//            }
//            else if(nowPrintState === "NULL")
//            {
//                machineStatus.machineStatusText =  "空闲"
//            }
//        }
//        function onPrintIDChange(printID)
//        {
//            machineStatus.serialNumber = printID;
//        }
//        function onLedTempChanged(temperature)
//        {
////            console.log("更新圆环状态");
//            machineStatus.statusLight = printStateqml.ledBrightness;
//            machineStatus.statusCurrent = printStateqml.ledCurrent;
//            machineStatus.statusResinTemp = printStateqml.resinTemperature;
//            machineStatus.statusTemp = temperature;
//        }
//    }

//    Connections//调节水平
//    {
//        target:adjustPlane
//        ignoreUnknownSignals: true
//        function onVoltageChange(voltage)
//        {
//            if(voltage)
//            {
//                applicationLoader.source = "qrc:/AdjustPlaneWindow.qml";
//                console.log("显示调整平衡窗口");
//            }
//            else
//            {
//                applicationLoader.source = "";
//                console.log("关闭调整平衡窗口");
//            }
//        }
//        function onPositionChange(str)
//        {
////            adjustPlane.showNewPosition(str).connect(applicationLoader.item.newPosition(str));
//            applicationLoader.item.newPosition(str);
//        }
//    }

//    InputPanel//全局虚拟键盘
//    {
//        id:vkb
//        visible:false
//        y:Qt.inputMethod.visible ? parent.height - vkb.height : parent.height
//        anchors.left: parent.left
//        anchors.right: parent.right
//        anchors.bottom: parent.bottom
//        Component.onCompleted:
//        {
//            VirtualKeyboardSettings.activeLocales = ["en_GB","zh_CN"]
//            VirtualKeyboardSettings.locale = "en_GB"
//        }
//        onActiveChanged:
//        {
//            if(!active) { visible = false; }
//        }
//    }
//}


///*##^##
//Designer {
//    D{i:1;anchors_width:0}D{i:3;anchors_width:800;anchors_x:724;anchors_y:171}
//}
//##^##*/
