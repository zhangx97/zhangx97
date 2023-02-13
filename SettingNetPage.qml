import QtQuick 2.0
import wifi.search 1.0
import net.State 1.0
import QtQuick.Controls 2.0

Item {
    Rectangle{
        id:netStateLabel
        anchors.fill: parent
        color: "#f2f2f7"
//        color: "red"
        radius: 10
        property bool clickFlag: true;

        NetStateModel{id:netStateInfo}

        Rectangle{
            id:netStateRec
            anchors.top:parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.topMargin: 10
            anchors.leftMargin: 30
            anchors.rightMargin: 30
            height: 110
            color:"white"
            ListView{
                id:netStateList
                anchors.fill: parent
                delegate: showNetState
                model: ListModel{
                    id:netModel;
                    Component.onCompleted: {
                        //ListElement不能使用常规的属性绑定，所以用这种方法
                        netModel.append({name:"互联网",connectState:netStateInfo.internetActive});
                        netModel.append({name:"局域网",connectState:netStateInfo.localNetActive});
                    }
                }
                clip: true
            }
            Component{
                id:showNetState
                Rectangle{
                    color: "white"
                    width: netStateList.width
                    height: 55
                    Text {
                        id: netName
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: 10
                        font.pointSize: 20
                        text: qsTr(name)
                    }
                    Text {
                        id: netstate
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        anchors.rightMargin: 30
                        font.pointSize: 20
                        color:"grey"
                        text: {connectState ? qsTr("已连接"):qsTr("未连接")}
                    }
                    //绘制分隔线
                    Canvas{
                        id:netDrawArea
                        anchors.bottom: parent.bottom

                        width:netStateList.width
                        height: 1
                        onPaint: {
                            var ctx = getContext("2d");
                            ctx.lineWidth = 1;
                            ctx.strokeStyle = "#c6c6c8";
                            ctx.beginPath();
                            ctx.moveTo(50,1);
                            ctx.lineTo(width,1);
                            //保存行程
                            ctx.stroke();
                        }
                    }
                }
            }
        }

        Rectangle{
            id:netWifiSettingRec
            anchors.top: netStateRec.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.topMargin: 60
            anchors.leftMargin: 30
            anchors.rightMargin: 30
            height: 55
            color:"white"
            Text {
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                font.pointSize: 20
                text: qsTr("WiFi")
            }
            Switch{
                id:wifiSwitch
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 30
                checked:wifiListInfo.WifiEnable

//                onClicked:{
//                    if(wifiSwitch.checked){
//                        netChoseWifiRec.visible = "true";
//                        netChoseWifiRec.anchors.bottomMargin = 50;
//                    }
//                    else{
//                        netChoseWifiRec.visible = "false";
//                         netChoseWifiRec.anchors.bottomMargin = 0;
//                    }
//                }
            }
        }

        Rectangle{
            id:netChoseWifiRec
            anchors.top: netWifiSettingRec.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom:wifiSwitch.checked? parent.bottom:netWifiSettingRec.bottom;
            anchors.topMargin: 60
            anchors.leftMargin: 30
            anchors.rightMargin: 30
            anchors.bottomMargin: wifiSwitch.checked? 10:0
            color:"white"


            //wifi的列表
            ListView{
                id:wifiList
                anchors.fill:parent
                model: WifiModel{id:wifiListInfo}
                delegate: showWifiListInfo
                clip: true
            }
            Component{
                id:showWifiListInfo
                //打包文字与分隔线
                Rectangle{
//                    anchors.top: parent.top
//                    anchors.right: parent.right
//                    anchors.left: parent.left
//                    anchors.leftMargin: 50
                    width: wifiList.width
                    height: 55
//                    color: "yellow"
                    //显示wifi名
                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.rightMargin: 100
                        font.pointSize: 20
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
                            anchors.fill: parent
                            source: {
                                if(wifiQuality<25)
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
                        id:choseWifiDrawArea
                        anchors.bottom: parent.bottom

                        width:wifiList.width
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
                    MouseArea
                    {
                        anchors.fill: parent
                        //点击之后加载wifi连接画面
                        onClicked:
                        {
                            if(netStateLabel.clickFlag)
                            {
                                //从loader处加载
                                settingNetPageLoader.source = "qrc:/WifiConnectPage.qml";
                                settingNetPageLoader.item.inputWifiName = wifiName;
                                settingNetPageLoader.item.marginHeight = 10;
                                netStateLabel.clickFlag = false;
                            }
                            else
                            {
                                wifiConnectWindow.source = "";
                                netStateLabel.clickFlag = true;
                            }
                        }
                    }
                }
            }
        }
    }
    Loader{
        id:settingNetPageLoader
//            anchors.top:applicationWindow.top
//            anchors.topMargin: 10
        x:-640
        y:-240
//            anchors.fill: parent.parent
//            width: applicationWindow.width
//            height: applicationWindow.height

    }
    Connections{
        target: settingNetPageLoader.item
        onCloseWifiConnectWindow:{
            settingNetPageLoader.source = "";
            netStateLabel.clickFlag = true;
        }
        onConnectWifi:
        {
            wifiListInfo.Name = name;
            wifiListInfo.PassWord = password;
            wifiListInfo.connectWifi();
        }
    }
}
