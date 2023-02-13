import QtQuick 2.0
import setting.item 1.0
import QtQuick.Controls 2.0

Rectangle {
    id:settingBackground
    width:1920
    height:1080
    color:"#7F2F4F4F"
    signal closeSettingWindow();
    signal startUpgrade(string version);
    signal startInstall();
    signal settingItemClicked(string modelName);
    signal protectScreenTime(string time);
    signal heatStatusChange(bool flag)

    //点击灰色区域关闭该窗口
    MouseArea
    {
        anchors.fill: parent
        onClicked: settingBackground.closeSettingWindow();
    }

    //窗口
    Rectangle{
        id:deviceMessageWindowArea
        width: 1366
        height: 768
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        color: "#f2f2f7"
        Text {
            anchors.top:parent.top
            anchors.topMargin: 60
            anchors.left: parent.left
            anchors.leftMargin: 30
            text: qsTr("设置")
            font.pointSize: 30
        }
        //为了点击窗口内部的时候，不会关闭窗口而设置
        MouseArea{
            anchors.fill: parent
        }
        //窗口左侧选项列表
        Rectangle{
            id:settingItemRec
            width: 300
            height: 240
            radius: 10
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 30
            color: "white"

            ListView{
                id:settingItemList
                clip:true
                anchors.fill: parent
                anchors.topMargin: 10
                anchors.bottomMargin: 10
                delegate: settingChoseMode
                model: SettingItemModel{id:settingItemDetail}
            }
            Component{
                id:settingChoseMode
                //窗口左侧选项列表中的项
                Rectangle {
                    id: detailItem
                    width: settingItemList.width
                    height: 55
                    color: detailItem.ListView.isCurrentItem ? "#007aff":"white"

                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            detailItem.ListView.view.currentIndex = index;
                            settingBackground.settingItemClicked(functionName);
                        }
                    }

                    Text {
                        id: settingItemText
                        color: detailItem.ListView.isCurrentItem ? "white":"black"
                        anchors.left: parent.left
                        anchors.leftMargin: 50
                        anchors.verticalCenter: parent.verticalCenter
                        font.pointSize: 20
                        text: functionName
                    }
                    Text {
                        id: itemState
                        color: detailItem.ListView.isCurrentItem ? "white":"grey"
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        anchors.rightMargin: 30
                        font.pointSize: 20
                        text: functionState
                    }
                    //绘制分隔线
                    Canvas{
                        id:settingDrawArea
                        anchors.bottom: parent.bottom

                        width:detailItem.width
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

        Loader{
            id:infoPage
            anchors.right: parent.right
            anchors.rightMargin: 30
            anchors.left:settingItemRec.right
            anchors.leftMargin: 30
            anchors.verticalCenter: parent.verticalCenter
            height: 600
            source:"qrc:/setting/SettingDeviceMessage.qml";
        }

        Connections
        {
            target: settingBackground
            ignoreUnknownSignals: true
            function onSettingItemClicked(modelName){
                console.log(modelName)
                if(modelName === "设备信息"){
                    //加载设备信息界面
                    console.log("加载设备");
                    infoPage.source = "qrc:/setting/SettingDeviceMessage.qml";
                }

                if(modelName === "网络"){
                    //加载网络信息界面
                    infoPage.source = "qrc:/setting/SettingNetPage.qml";
                }

                if(modelName === "材料"){
                    //加载材料界面
                    infoPage.source ="qrc:/setting/SettingMaterialPage.qml";
                }

                if(modelName === "检查更新"){
                    //加载更新界面
                    infoPage.source ="qrc:/setting/SettingCheckUpdate.qml";
                }
            }
        }

        Connections
        {
            target: infoPage.item
            ignoreUnknownSignals: true
            function onDownloadUpdate(version)
            {
                settingBackground.startUpgrade(version);
            }
            function onStartInstall()
            {
                settingBackground.startInstall();
            }
            function onScreenTimeChange(time)
            {
                console.log("向主页面发送信号"+time);
                settingBackground.protectScreenTime(time);
            }
            function onHeatSwitchChange(flag)
            {
                settingBackground.heatStatusChange(flag);
            }
        }
    }
}
