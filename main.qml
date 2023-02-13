import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.4
import QtQuick.VirtualKeyboard 2.2
import QtQuick.VirtualKeyboard.Settings 2.2
import qmlControl.item 1.0
import qmlState.item 1.0
import adjustPlaneVoltage 1.0
import screenProtect 1.0

ApplicationWindow {
    id: applicationWindow
    objectName: "rootObject"
    visible: true
    x:1920
    width: 1920
    height: 1080
    flags:Qt.FramelessWindowHint
    color: "#f2f2f7"
    property string slicePath: ""//需要被打印的slicepack名称

    Component.onCompleted:
    {
//        var component = Qt.createComponent("ProjectionArea.qml");
//        var win = component.createObject(null);
//        win.show();
//        win.closing.connect(function() { console.log('窗口关闭了') })
//        printImagePage.imgPath = "../projection/blackscreen.png"
        printImagePage.show();
    }

    AdjustPlane
    {
        id:adjustPlane
//        signal showNewPosition(string position);
    }

    ProjectionArea//投影窗口
    {
        objectName: "printImagePage";
        id:printImagePage
        imgPath: "../projection/blackscreen.png"
    }

    TitleArea{//标题
        id:titleWindow
        height: 110
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.topMargin: 40
        anchors.leftMargin: 50
        anchors.rightMargin: 50
    }

    StatusArea{//状态页面
        id:machineStatus
        anchors.left: parent.left
        anchors.leftMargin: 50
        anchors.top: titleWindow.bottom
        anchors.topMargin: 70
        machineStatusText:"空闲"
        width: 640
        height: 500
    }

    SlicePackArea{//切层图包和开始打印
        id:printArea
        anchors.left: machineStatus.right
        anchors.leftMargin: 10
        height: 500
        anchors.top: titleWindow.bottom
        anchors.topMargin: 70
        anchors.right: parent.right
        anchors.rightMargin: 50
    }

    //wifi/设置这些按钮以后可以通过repeater来加载
    WifiFunctionBottom//wifi页面
    {
        id:wifiBottom
        anchors.left: parent.left
        anchors.leftMargin: 50
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 50
        width:170
        height: 240
        MouseArea
        {
            anchors.fill: parent
            onClicked:
            {
                applicationLoader.source = "qrc:/wifi/WifiPage.qml";
            }
        }
    }

    SettingBottom//设置页面
    {
        id:settingBottom
        anchors.left: wifiBottom.right
        anchors.leftMargin: 50
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 50
        width:170
        height: 240
        MouseArea
        {
            anchors.fill: parent
            onClicked:
            {
                applicationLoader.source = "qrc:/setting/SettingPage.qml";
            }
        }
    }

    LogBottom
    {
        id:logBottom
        anchors.left: settingBottom.right
        anchors.leftMargin: 50
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 50
        width: 170
        height: 240
        MouseArea
        {
            anchors.fill: parent
            onClicked:
            {
                applicationLoader.source = "qrc:/log/ShowLogPage.qml";
            }
        }
    }

    Loader//加载应用界面
    {
        id:applicationLoader
        anchors.fill:parent
    }

    Loader//加载确认框
    {
        id:remindWindow
        anchors.fill: parent
    }

    Loader//加载锁屏界面
    {
        id:screenLock
        anchors.fill: parent
    }

    QMLControl//qml打印控制类
    {
        id:qmlControl
    }

    QMLState
    {
        id:printStateqml
    }

    ScreenSaver
    {
        id:screenSaver
    }
    //连接后台检查鼠标活动的类
    Connections
    {
        target: screenSaver
        function onStartBlack()
        {
            screenLock.source = "qrc:/screenProtect/ScreenProtect.qml"
        }
    }
    //连接屏幕保护的loader
    Connections
    {
        target: screenLock.item
        function onCancelScreen()
        {
            screenLock.source = "";
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
    //连接提示框发出的信号
    Connections
    {
        ignoreUnknownSignals: true
        target:remindWindow.item
        //弹窗点击确认后，接受到的指令与对应的操作
        //指令都是小写开头，打印指令为print
        function onClickOk(type)
        {
            if(type === "print")
            {
                remindWindow.source = "";
                //发送开始打印指令
                qmlControl.printStart(slicePath)
                console.log("主程序页面clickok信号触发");
            }
            if(type === "upgrade")
            {
                remindWindow.source = "";
                qmlControl.startUpgrade();
            }
        }
        function onCloseConfirmWindow()
        {
            remindWindow.source = "";
            console.log("主程序页面关闭提示窗信号触发");
        }
        function onStartInstall()
        {
            qmlControl.zipReadySlot();
        }
    }

    //显示弹窗
    Connections
    {
        target: printArea
        ignoreUnknownSignals: true
        function onStartPrint(slicePackPath){
            console.log("qml信号中传递的路径：",slicePackPath)
            slicePath = slicePackPath
            remindWindow.source = "ConfirmWindow.qml"
            remindWindow.item.title = "确认开始打印"
            remindWindow.item.content = "是否确认开始打印？"
            remindWindow.item.cmdType = "print"
        }
        function onChangePackInfo(array)
        {
            //这里接收数组,显示到状态框里
            machineStatus.slicepackInfo = array;
        }
    }

    //连接qml内的打印控制类
    Connections
    {
        target:qmlControl
        ignoreUnknownSignals: true
        function onChangePhoto(path)
        {
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
        function onStopPrint()
        {
            console.log("qml界面收到停止打印信号");
            applicationLoader.item.printFinish();
        }
    }

    Connections//连接qml状态类
    {
        target: printStateqml
        ignoreUnknownSignals: true
        function onPrintStateChanged(nowPrintState)
        {
            if(nowPrintState === "start" || nowPrintState === "continue")
            {
                machineStatus.machineStatusText = "开始打印"
            }
            else if(nowPrintState === "stop")
            {
                machineStatus.machineStatusText =  "正在停止"
            }
            else if(nowPrintState === "heat")
            {
                machineStatus.machineStatusText =  "加热中"
            }
            else if(nowPrintState === "pause")
            {
                machineStatus.machineStatusText =  "暂停"
            }
            else if(nowPrintState === "NULL")
            {
                machineStatus.machineStatusText =  "空闲"
            }
        }
        function onPrintIDChange(printID)
        {
            machineStatus.serialNumber = printID;
        }
        function onLedTempChanged(temperature)
        {
//            console.log("更新圆环状态");
            machineStatus.statusLight = printStateqml.ledBrightness;
            machineStatus.statusCurrent = printStateqml.ledCurrent;
            machineStatus.statusResinTemp = printStateqml.resinTemperature;
            machineStatus.statusTemp = temperature;
        }
    }

    Connections//调节水平
    {
        target:adjustPlane
        ignoreUnknownSignals: true
        function onVoltageChange(voltage)
        {
            if(voltage)
            {
                applicationLoader.source = "qrc:/AdjustPlaneWindow.qml";
                console.log("显示调整平衡窗口");
            }
            else
            {
                applicationLoader.source = "";
                console.log("关闭调整平衡窗口");
            }
        }
        function onPositionChange(str)
        {
//            adjustPlane.showNewPosition(str).connect(applicationLoader.item.newPosition(str));
            applicationLoader.item.newPosition(str);
        }
    }

    InputPanel//全局虚拟键盘
    {
        id:vkb
        visible:false
        y:Qt.inputMethod.visible ? parent.height - vkb.height : parent.height
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        Component.onCompleted:
        {
            VirtualKeyboardSettings.activeLocales = ["en_GB","zh_CN"]
            VirtualKeyboardSettings.locale = "en_GB"
        }
        onActiveChanged:
        {
            if(!active) { visible = false; }
        }
    }
}


/*##^##
Designer {
    D{i:1;anchors_width:0}D{i:3;anchors_width:800;anchors_x:724;anchors_y:171}
}
##^##*/
