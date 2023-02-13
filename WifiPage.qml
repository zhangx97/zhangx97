import QtQuick 2.0
import wifi.search 1.0
import QtQuick.Controls 2.0
//wifi列表页

Rectangle {
    id:wifiBackground
    width:1920
    height:1080
    color:"#7F2F4F4F"
    signal closeWifiWindow();
    property bool clickFlag: true;

    MouseArea
    {
        anchors.fill: parent
        onClicked: {
            wifiBackground.closeWifiWindow();
        }
    }
    Rectangle
    {
        id:wifiMessageList
        height:760
        width:500
        radius: 50

        anchors.centerIn: parent
        color:"white"
        ListView{
            id:listView
            anchors.fill: parent
            anchors.margins: 20
            clip: true
            model: WifiModel{id : wifiMessageModel}
            delegate: numberDelegate
            spacing: 2
            highlightFollowsCurrentItem:true
        }

        Component{
            id: numberDelegate
            Item {//listview中的每一项
                id:packRange

                width: ListView.view.width
                height: 80
                //打包文字与分隔线
                Rectangle{
                    id:wifiText
                    anchors.top: parent.top
                    anchors.right: parent.right
                    anchors.left: parent.left
                    anchors.leftMargin: 50
                    height:80
                    color:"white"

                    //显示wifi名
                    Text {
                        id:packName
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.rightMargin: 100
                        font.pointSize: 17
                        text: wifiName
                        elide: Text.ElideMiddle
                    }

                    //显示wifi信号图像
                    Rectangle
                    {
                        anchors.right: parent.right
                        anchors.rightMargin: 50
                        anchors.verticalCenter: parent.verticalCenter
                        width:40
                        height: 40
                        Image {
                            id: name
                            anchors.fill: parent
                            source: {
                                if(wifiQuality == 0)
                                {
                                    ""
                                }
                                else if(wifiQuality<25 && wifiQuality > 0)
                                {
                                    "qrc:/img/img/signal0.png"
                                }
                                else if(wifiQuality >= 25 && wifiQuality < 30)
                                {
                                    "qrc:/img/img/signal1.png"
                                }
                                else if(wifiQuality >= 30 && wifiQuality < 40)
                                {
                                    "qrc:/img/img/signal2.png"
                                }
                                else
                                {
                                    "qrc:/img/img/signal3.png"
                                }
                            }
                        }
                    }

                    //绘制分隔线
                    Canvas{
                        id:wifiDrawArea
                        anchors.bottom: parent.bottom

                        width:packRange.width
                        height: 1
                        onPaint: {
                            var ctx = getContext("2d")
                            ctx.strokeStyle = "#c6c6c8"
                            ctx.beginPath()
                            ctx.moveTo(0,1)
                            ctx.lineTo(width-50,1)
                            //保存行程
                            ctx.stroke()
                        }
                    }
                }
                MouseArea
                {

                    anchors.fill: parent
                    //点击之后加载wifi连接画面
                    onClicked:
                    {
                        if(wifiBackground.clickFlag)
                        {
                            //从loader处加载
                            wifiConnectWindow.source = "qrc:/wifi/WifiConnectPage.qml";
                            wifiConnectWindow.item.inputWifiName = wifiName;
                            wifiBackground.clickFlag = false;
                        }
                        else
                        {
                            wifiConnectWindow.source = "";
                            wifiBackground.clickFlag = true;
                        }
                    }
                }
            }
        }
    }

    Loader{
        id:wifiConnectWindow
        anchors.fill: wifiBackground
    }

    Connections
    {
        target: wifiMessageModel
        function onNetCheckResult(result)
        {
            if(result)
            {
//                wifiConnectWindow.source = "";
                wifiConnectWindow.item.resultText = "连接成功"
            }
            else
            {
                wifiConnectWindow.item.resultText = "密码错误,连接失败"
            }
        }
    }

    Connections
    {
        target:wifiConnectWindow.item
        function onConnectWifi(name,password)
        {
            wifiMessageModel.Name = name;
            wifiMessageModel.PassWord = password;
            wifiMessageModel.connectWifi();
        }
        function onCloseWifiConnectWindow()
        {
            wifiConnectWindow.source = "";
            wifiBackground.clickFlag = true;
        }
    }
}
