import QtQuick 2.0

Item {
    id:unlockPage
    property real firstY
    property real lastY
    signal unlockStart()

    Rectangle
    {
        anchors.fill: parent
        Image
        {
            anchors.fill: parent
            //这里随便塞张图片
        }
        Text {
            id: unlockName
            text: qsTr("右滑以解锁")
        }
        MouseArea
        {
            anchors.fill: parent
            //获取鼠标的位置,来判断是否解锁
            onPressed:
            {
                unlockPage.firstY = mouseY
                console.log("点击的x轴位置:"+unlockPage.firstY);
            }
            onReleased:
            {
                unlockPage.lastY = mouseY
                console.log("离开时的x轴位置:"+unlockPage.lastY);
                console.log("相减的结果:"+(unlockPage.firstY - unlockPage.lastY))
                //如果滑动距离大于200像素,就解锁
                if(unlockPage.firstY - unlockPage.lastY > 200)
                {
                    //发送信号,让ScreenProtect的loader释放自己
                    console.log("解锁成功")
                    unlockPage.unlockStart();
                }
            }
        }
    }
}
