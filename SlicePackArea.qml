import QtQuick 2.0
import QtQuick.Controls 2.15
import slicePack.item 1.0
import QtGraphicalEffects 1.0
//显示切层图包的页面

//这个控件用来放置切层图包
Item{
    id:slicepackItem
    signal changeImg();
    signal startPrint(string packPath);
    signal changePackInfo(var array);
    property var slicepackInfo: []
    Rectangle{
        id:slicePackListView
//        anchors.fill: parent
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        width: parent.width/2-10
        color: "white"
        radius:30

        Component.onCompleted:
        {
            slicepackItem.slicepackInfo[0] = slicePack.material;
            slicepackItem.slicepackInfo[1] = slicePack.thickness;
            slicepackItem.slicepackInfo[2] = slicePack.curingTime;
            slicepackItem.changePackInfo(slicepackItem.slicepackInfo)
        }

        ListView{
            id:sliceList
            anchors.fill: parent
            anchors.margins: 20
            clip: true
            model: SlicePackModel{
                id : slicePack
            }
            delegate: numberDelegate
            spacing: 2
            maximumFlickVelocity: 1600
            highlightFollowsCurrentItem:true
        }

        Component{
            id: numberDelegate//ListView内单个项的数据

            Rectangle {
                id:packRange
                width: ListView.view.width
                height: 80
                radius: 20
                color: packRange.ListView.isCurrentItem ? "#007aff":"white"
                MouseArea{
                    id:signalEmit
                    anchors.fill: parent
                    onClicked: {
                        packRange.ListView.view.currentIndex = index;
                        slicePack.packPath = slicePathCpp;
                        slicePack.packName = packNameCpp;
                        slicePack.initInfo();

                        slicepackItem.slicepackInfo[0] = slicePack.material;
                        slicepackItem.slicepackInfo[1] = slicePack.thickness;
                        slicepackItem.slicepackInfo[2] = slicePack.curingTime;
                        slicepackItem.changePackInfo(slicepackItem.slicepackInfo)

                        slicepackItem.changeImg();
                        console.log("slicepack名："+packNameCpp);
                    }
                }
                //打包文字与分隔线
                Item{
                    anchors.fill: parent
                    anchors.left: parent.left
                    anchors.leftMargin: 50
                    Text {
                        id:slicePackName
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        font.pixelSize: 30
                        text: packNameCpp
                        color: packRange.ListView.isCurrentItem ? "white":"black"
                    }
                    //绘制分隔线
                    Canvas{
                        anchors.top: parent.bottom
                        width:packRange.width
                        height: 1
                        onPaint: {
                            var ctx = getContext("2d")
                            ctx.strokeStyle = "#c6c6c8"
                            ctx.beginPath()
                            ctx.moveTo(0,1)
                            ctx.lineTo(width,1)
                        }
                    }
                }
            }
        }//ListView内单个项的数据
    }
    Rectangle{//预览图
        id:imgPreview
        anchors.right:  parent.right
        anchors.top: parent.top
        anchors.bottom: startPrintBottom.top
        anchors.bottomMargin: 10
        width: parent.width/2-10
        radius: 30
        color: "#f2f2f7"

        SwipeView{
            id : swipeView//预览图切换器
            anchors.fill: parent
            currentIndex: indicator.currentIndex
            orientation: Qt.Horizontal
            interactive: true
            clip: true

            Rectangle{
                color: "#f2f2f7"
                radius: 20
                Image {
                    id: name1
                    source: slicePack.returnFrontImg()
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
            Rectangle{
                color: "#f2f2f7"
                radius: 20
                Image {
                    id: name2
                    source: slicePack.returnBackImg()
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

        PageIndicator{//预览图切换指示圈
            id:indicator
            count:swipeView.count
            currentIndex: swipeView.currentIndex
            interactive: true
            anchors.bottom: swipeView.bottom
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
    Rectangle{//开始打印按钮
        id:startPrintBottom//开始打印按钮
        anchors.bottom: parent.bottom
        anchors.left: slicePackListView.right
        anchors.leftMargin: 20
        anchors.right: parent.right
        height: parent.height/5
        radius:30
        color: "#007aff"

        Text {
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            font.pointSize: 30
            color: "white"
            text: qsTr("开始打印")
        }
        MouseArea{
            anchors.fill: parent
            onClicked:
            {
                slicepackItem.startPrint(slicePack.packPath)//这里弹窗提示是否确认开始打印
                console.log("qml中选中包的路径：",slicePack.packPath)
            }
        }
    }
    Connections
    {
        target: slicepackItem
        function onChangeImg(){
            name1.source = slicePack.returnFrontImg();
            name2.source = slicePack.returnBackImg();
        }
    }
}




/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
