import QtQuick 2.0

Rectangle
{
    id:choseBackground
    anchors.fill: parent
    color:"#7F2F4F4F"
    signal closeChosePage();
    signal sendChoseItem(string item);

    property var dataList: []
    property alias choseData: choseBackground.dataList
    property string chosedItem: ""

    MouseArea
    {
        anchors.fill: parent
        onClicked:
        {
            choseBackground.closeChosePage();
        }
    }
    //可选列表
    Rectangle
    {
        id:slideChoseList
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        height: 400
        width: 300
        radius: 10

        ListView
        {
            id:choseItemList
            anchors.top: parent.top
            anchors.topMargin: 20
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 20
            delegate:choseItem
            model:choseBackground.dataList
            clip: true
        }
        Component
        {
            id:choseItem
            Rectangle
            {
                id:signalChosedItem
                width: choseItemList.width
                height: 80
                color: signalChosedItem.ListView.isCurrentItem ? "#007aff":"white"
                MouseArea
                {
                    anchors.fill: parent
                    onClicked:
                    {
                        //记录选中的项目,并改变颜色
                        chosedItem = choseBackground.dataList[index];
                        signalChosedItem.ListView.view.currentIndex = index;
                        choseBackground.sendChoseItem(chosedItem);
                        delayClose.running = true;
                    }
                }
                Text {
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter

                    text:choseBackground.dataList[index]
                    color: signalChosedItem.ListView.isCurrentItem ? "white":"black"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.pointSize: 25
                }
            }
        }
    }
    Timer
    {
        id:delayClose
        interval: 300
        running:false
        repeat: false
        onTriggered:
        {
            choseBackground.closeChosePage();
        }
    }
    //确认按钮
//    Rectangle
//    {
//        id:confirmClickWindow
//        MouseArea
//        {
//            anchors.fill:parent
//            onClicked:
//            {
//                choseBackground.sendChoseItem(chosedItem);
//                choseBackground.closeChosePage();
//            }
//        }
//        Text {
//            anchors.horizontalCenter: parent.horizontalCenter
//            anchors.verticalCenter: parent.verticalCenter
//            id: confirnBotton
//            text: qsTr("确认")
//            font.pointSize: 10
//        }
//    }
}
