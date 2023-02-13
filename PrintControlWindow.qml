import QtQuick 2.0
import qmlState.item 1.0
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.0

Rectangle
{
    id:bottomItem
    signal stopPrint();
    signal closePrintWindow();
    property Item loader

    QMLState
    {
        id:qmlPrintState
    }

    Rectangle//界面背景
    {
        height:1080
        width:1920

        Rectangle//显示在打印中的图片
        {
            anchors.fill: parent
            SwipeView{//预览图切换器
                id : swipeView
                anchors.fill: parent
                currentIndex: indicator.currentIndex
                orientation: Qt.Horizontal
                interactive: true
                clip: true

                Rectangle{//图片1
                    color: "#f2f2f7"
                    radius: 20
                    Image {
                        id: name1
                        source: qmlPrintState.backImg
                        anchors.fill: parent
                        visible: false
                    }
                    OpacityMask{
                        anchors.fill: name1
                        source:name1
                        maskSource:Rectangle{
                            width:name1.width
                            height: name1.height
                            radius: 20
                            visible: false
                        }
                    }
                }
                Rectangle{//图片2
                    color: "#f2f2f7"
                    radius: 20
                    Image {
                        id: name2
                        source: qmlPrintState.frontImg
                        anchors.fill: parent
                        visible: false
                    }
                    OpacityMask{
                        anchors.fill: name2
                        source:name2
                        maskSource:Rectangle{
                            width:name2.width
                            height: name2.height
                            radius: 20
                            visible: false
                        }
                    }
                }
            }
            PageIndicator{//预览图指示圈
                id:indicator
                count:swipeView.count
                currentIndex: swipeView.currentIndex
                interactive: true
                anchors.bottom: swipeView.bottom
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }

        Rectangle//控制面板
        {
            id:controlPanel
//                anchors.fill: parent
            anchors.verticalCenter:parent.verticalCenter
                color:"#7fafbcbc"
            width:parent.width/3
            height: parent.height/3
//            opacity: 0.8
            x:100
            radius: 30
            border.color:"white"
            border.width:2
            Item//控制区左侧进度条
            {
                id: item1
                width:parent.width/2
                height: parent.height
                anchors.left: parent.left
                PrintStatusPanel
                {
                    id:printPanel
                    signal panelRepaint();
                    anchors.fill:parent

//                    propertyName:"空闲" //由model决定
//                    propertyNumberValue:qmlPrintState.finishCount//由model输出当前层数
                    panelColor:"#34c759"
//                    maxValue:qmlPrintState.layerCount//由model输出最大层数
//                    haloPercent:qmlPrintState.printPercent
                    size:260
                }
            }
            Item//控制区右侧按钮与信息
            {
                id: item2
                anchors.right: parent.right
                width:parent.width/2
                height: parent.height

                ColumnLayout
                {
                    width: parent.width/2
                    height: 350
                    spacing:3
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter

                    Rectangle//（继续/暂停）按钮
                    {
                        id:printPause
                        height: 60
                        radius: 10
                        Layout.leftMargin: parent.width/2-width/2
                        width:180
                        color: "#007aff"
                        Text {
                            id: pauseAndContinue
                            color: "#ffffff"
                            text: qsTr("暂停")
                            anchors.fill: parent
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            font.pointSize: 16
                        }
                        MouseArea
                        {
                            anchors.fill: parent
                            //控制暂停/继续
                            onClicked:
                            {
                                //判断是暂停还是继续
                                if(pauseAndContinue.text === "继续")
                                {
                                    printPanel.propertyName = "开始打印";
                                    pauseAndContinue.text = "暂停";
                                    qmlPrintState.printContinue();

                                }
                                else if(pauseAndContinue.text === "暂停")
                                {
                                    printPanel.propertyName = "暂停";
                                    pauseAndContinue.text = "继续";
                                    qmlPrintState.pause();
                                }
                            }
                        }
                    }
                    Rectangle//停止按钮
                    {
                        id:printStop
                        height: 60
                        radius: 10
                        Layout.leftMargin: parent.width/2-width/2
                        width:180
                        color: "#007aff"

                        Text {
                            id: stop
                            color: "#ffffff"
                            text: qsTr("停止")
                            anchors.fill: parent
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            font.pointSize: 16
                        }
                        MouseArea
                        {
                            id:clickStopPrint
                            anchors.fill: parent
                            //停止打印
                            onClicked: {
                                bottomItem.stopPrint();
                            }
                            //显示停止打印提示栏，按确定后返回主界面
                        }
                    }

                    Item
                    {
                        id:timeRemain
                        height: 60
                        Layout.leftMargin: parent.width/2-width/2
                        width:180

                        Text {
                            id: remainingTime
//                            text: "剩余时间："+qmlPrintState.remainTime//计算剩余时间
                            anchors.fill: parent
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            font.pointSize: 12
//                                opacity: 0.5
                        }
                    }//显示剩余时间
                }
            }
        }
    }
    Loader//显示提示窗口
    {
        id:infoWindowLoader
        anchors.fill:parent
    }

    Connections//连接C++发出的信号，控制进度条与信息面板
    {
        target: qmlPrintState
        function onPrintStateChanged(nowPrintState)
        {
            showPrintInfo(nowPrintState);
        }
        function onBrightnessChanged(ledBright)
        {
            //
        }
        function onTemperatureChanged(ledTemp)
        {
            //
        }
        function onLayerCountChanged(allLayerCount)
        {
            printPanel.maxValue = allLayerCount
        }
        function onFinishCountChanged(nowFinishCount)
        {
            printPanel.propertyNumberValue = nowFinishCount
        }
        function onLayerThicknessChanged(layThick)
        {
            //
        }
        function onRemainTimeChanged(remainTime)
        {
            remainingTime.text = "剩余时间："+remainTime
        }
        function onPrintPercentChanged(printPercent)
        {
            printPanel.haloPercent = printPercent;
            printPanel.panelRepaint.connect(printPanel.haloRepaint);
            printPanel.panelRepaint();

            var tempValue = (printPercent*100).toFixed(2);
            printPanel.percentNumberValue = tempValue+"%"
        }
    }

    Connections//连接停止打印按钮的信号
    {
        target:bottomItem
        function onStopPrint()
        {
            infoWindowLoader.source ="qrc:/ConfirmWindow.qml"
            infoWindowLoader.item.cmdType = "stopPrint"
            infoWindowLoader.item.title = "停止打印"
            infoWindowLoader.item.content = "是否确定要停止打印？"
        }
    }

//    Connections
//    {
//        target: printPause
//        function
//    }

    Connections//连接loader加载对象的信号
    {
        target:infoWindowLoader.item
        function onCloseConfirmWindow()
        {
            infoWindowLoader.source = "";
            console.log("打印详情页关闭提示窗信号触发");
        }
        function onClickOk(type)
        {
            if(type === "stopPrint")
            {
                qmlPrintState.stop();
                infoWindowLoader.source = "";
                bottomItem.closePrintWindow();
            }
        }
        function onCloseInfoWindow()
        {
            infoWindowLoader.source = "";
            bottomItem.closePrintWindow();
        }
    }

    function printFinish()
    {
        console.log("打印控制页面弹窗提醒打印结束");
        infoWindowLoader.source ="qrc:/InfoWindow.qml"
//        infoWindowLoader.item.cmdType = "stopPrint"
        infoWindowLoader.item.title = "打印完成"
        infoWindowLoader.item.content = "打印已完成，即将退出该界面"
    }

    function showPrintInfo(state)//切换进度条中的打印状态
    {
        if(state == "start" || state == "continue")
        {
            printPanel.propertyName = "开始打印"
        }
        else if(state == "stop")
        {
            printPanel.propertyName =  "停止"
            bottomItem.stopPrint();
        }
        else if(state == "heat")
        {
            printPanel.propertyName =  "加热中"
        }
        else if(state == "pause")
        {
            printPanel.propertyName =  "暂停"
        }
        else if(state == "NULL")
        {
            printPanel.propertyName =  "空闲"
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;formeditorZoom:0.5;height:480;width:640}D{i:1}D{i:6}D{i:9}D{i:5}
D{i:17}D{i:4}D{i:19}D{i:18}D{i:3}D{i:2}D{i:20}
}
##^##*/
