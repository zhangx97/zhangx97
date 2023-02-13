import QtQuick 2.0

Item {
    id:screenShowBlack
    signal cancelScreen();
    Rectangle
    {
        id:screenProtect
        anchors.fill: parent
        color: "black"
        MouseArea
        {
            anchors.fill: parent
            onClicked:
            {
                //点击后显示锁屏界面
                showLockPage.source = "qrc:/screenProtect/UnlockPage.qml"
            }
        }
    }
    Loader
    {
        id:showLockPage
        anchors.fill: parent
    }
    Connections
    {
        target: showLockPage.item
        function onUnlockStart()
        {
            showLockPage.source = "";
            console.log("锁屏页面关闭");
            screenShowBlack.cancelScreen();
        }
    }
}
