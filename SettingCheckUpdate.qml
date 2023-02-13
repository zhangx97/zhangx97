import QtQuick 2.0
import softUpdate 1.0

Item {
    id:updatePage
    signal downloadUpdate(string version);
    signal startInstall();
    property string version: ""
    property bool firstClick: true
    property bool installPackExist: false

    height: parent.height
    width:parent.width

    Component.onCompleted:
    {
        console.log("开始检查新版本");

        if(softUpdate.packExist)//如果存在安装包，就显示安装选项
        {
            updatePage.installPackExist = true;
            showStartInstall();
        }
        else
        {
            if(softUpdate.upgradeStatus)//如果正在下载，就继续显示进度
            {
                refreshUpdate.running = true;
                softUpdate.clickDownload();
            }
            else//检查是否有新版本
            {
                softUpdate.checkNewVersion();
            }
            console.log("结束检查新版本");
        }
    }

    UpdateSoft
    {
        id:softUpdate
    }
    Rectangle
    {
        id:checkUpdateBackground
        height: parent.height
        width:parent.width
        color: "#f2f2f7"

        Rectangle
        {
            id:gifBOx
            anchors.top: parent.top
            anchors.topMargin: 100
            anchors.left: parent.left
            anchors.leftMargin: 100
            anchors.right: parent.right
            anchors.rightMargin:100
            height: 300
            color: "white"
            radius: 20

//            AnimatedImage
//            {
//                anchors.fill: parent
//                id:updateGifPlay

//                function onCurrentFrameChanged()
//                {
//                    if(updateGifPlay.currentFrame == updateGifPlay.frameCount)
//                    {
//                        //播放完后重新循环播放
//                    }
//                }
//            }
        }
        Text
        {
            id:showNewVersion
            anchors.top: gifBOx.bottom
            anchors.topMargin: 20
            anchors.left:parent.left
            anchors.leftMargin: 100
            text: ""
            font.pointSize: 15
        }
        //更新按钮
        Rectangle
        {
            id:downloadBottom

            anchors.top:showNewVersion.bottom
            anchors.topMargin: 20
            anchors.left: parent.left
            anchors.leftMargin: 100
            anchors.right: parent.right
            anchors.rightMargin: 100

            color: "white"
            radius: 20

            MouseArea
            {
                anchors.fill: parent
                onClicked:
                {
                    if(updatePage.installPackExist)
                    {
                        if(firstClick)
                        {
                            updatePage.startInstall();
                            firstClick = false;
                        }
                    }
                    else
                    {
                        if(firstClick)
                        {
                            updatePage.downloadUpdate(updatePage.version);//发送信号到主窗口，开始下载
                            refreshUpdate.running = true;
                            softUpdate.clickDownload();
                            firstClick = false;
                        }
                    }

                }
            }
            //下载并安装按钮
            Text
            {
                id: downloadBottomText
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: qsTr("")
                font.pointSize: 15
            }
            Text
            {
                id:downloadProgress
                anchors.left: parent.left
                anchors.leftMargin: 110
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: ""
                font.pointSize: 15
            }
        }
    }

    Timer
    {
        id:refreshUpdate
        property int count: 0
        interval: 1000
        running:false
        repeat: true
        onTriggered:
        {
            refreshUpdate.count += 1;
            if(refreshUpdate.count == 60)
            {
                refreshUpdate.count = 0;
            }
            if(refreshUpdate.count % 3 == 0)
            {
                downloadBottomText.text = "正在下载.";
            }
            else if(refreshUpdate.count % 3 == 1)
            {
                downloadBottomText.text = "正在下载..";
            }
            else
            {
                downloadBottomText.text = "正在下载...";
            }
        }
    }
    //显示下载按钮
    function showDownloadBottom()
    {
        downloadBottom.height = 50;
//        downloadBottom.width = gifBOx.width;
        downloadBottomText.text = "下载并安装";
    }

    function showStartInstall()
    {
        downloadBottom.height = 50;
        downloadBottomText.text = "开始安装";
    }

    //连接update类
    Connections
    {
        target: softUpdate

        function onNewVersion(find,newVersion)
        {
            if(find)
            {
                //有更新，把gif换成检测到新版本，并显示下载按钮
                showDownloadBottom();
                updatePage.version = newVersion;
                showNewVersion.text = "最新版本:"+newVersion;
            }
            else
            {
                //没更新，gif换成已经是最新
            }
        }

        function onPercentChange(percent)
        {
//            console.log("显示下载百分比");
            if(percent === 1 )
            {
                downloadBottomText.text = "开始安装";
            }
            else
            {
                downloadProgress.text = (percent*100).toFixed(2)+" %"
            }
        }
    }
}
