import QtQuick 2.0
import device.item 1.0
import QtQuick.Controls 2.0

Item {
    id:devicepage
    signal screenTimeChange(string newTime);
    signal heatSwitchChange(bool flag);

    Rectangle{
        anchors.fill: parent
        color: "#f2f2f7"

        ListView{
            id:deviceListView
            anchors.fill: parent
            anchors.topMargin: 10
            anchors.bottomMargin: 10
            anchors.leftMargin: 30
            anchors.rightMargin: 30
            delegate:detailComponent
            model:DeviceItemModel{id:deviceDetail}
            clip: true
            section.property: "infoSection"
            section.criteria: ViewSection.FullString
            section.delegate: deviceSection
        }

        Component{
            id:detailComponent
            Rectangle{
                id:deviceDetailItem
                width: deviceListView.width
                height: 55
                color:deviceDetailItem.ListView.isCurrentItem ? "#007aff":"white"
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        deviceDetailItem.ListView.view.currentIndex = index;
                        if(infoName == "休眠时间")
                        {
                            choseSleepTime.source = "qrc:/setting/ChoseItemPage.qml";
                            choseSleepTime.item.choseData = ["1分钟","2分钟","5分钟","10分钟","30分钟","永不"]
                        }
                        if(infoName == "加热开关")
                        {
                            //点击控制开关
                            if(deviceInfoContent.text === "开")
                            {
                                deviceInfoContent.text = "关";
                                devicepage.heatSwitchChange(false);
                                deviceDetail.refresh();
                            }
                            else
                            {
                                deviceInfoContent.text = "开";
                                devicepage.heatSwitchChange(true);
                                deviceDetail.refresh();
                            }
                        }
                    }
                }
                Text {
                    id: deviceInfoName
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    color: deviceDetailItem.ListView.isCurrentItem ? "white":"black"
                    font.pointSize: 20
                    text: infoName
                }
                Text {
                    id: deviceInfoContent
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 30
                    color: deviceDetailItem.ListView.isCurrentItem ? "white":"grey"
                    font.pointSize: 20
                    text: infoContent
                }
                //绘制分隔线
                Canvas{
                    id:deviceDrawArea
                    anchors.bottom: parent.bottom

                    width:deviceListView.width
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

        Component{
            id:deviceSection
            Rectangle{
                width: deviceListView.width
                height: 60
                color: "#f2f2f7"
            }
        }

        Loader
        {
            id:choseSleepTime
            anchors.fill: parent
        }

        Connections
        {
            target: choseSleepTime.item

            function onSendChoseItem(item)
            {
                devicepage.screenTimeChange(item);
                console.log("屏保时间设定值:"+item);
            }
            function onCloseChosePage()
            {
                choseSleepTime.source = "";
                deviceDetail.refresh();
            }
        }
    }
}
