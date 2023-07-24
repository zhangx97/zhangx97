import QtQuick 2.0
import QtQuick.Controls 2.0
//连接wifi的窗口

Rectangle{
    id:wifiConnectBackground
    width:1920
    height:1080
    color:"#7F2F4F4F"
    signal closeWifiConnectWindow();
    signal connectWifi(string name,string password)
    property alias inputWifiName:wifiNameText.wifiName
    property alias marginHeight: wifiMessageRectangle.windowHeight
    property alias resultText: wifiConnectBackground.connectText
    property string connectText: ""

    MouseArea
    {
        anchors.fill: parent
        onClicked:
        {
            closeWifiConnectWindow();
        }
    }

    Rectangle {
        id:wifiMessageRectangle

        property int windowHeight:160
        anchors.top: wifiConnectBackground.top
        anchors.topMargin: windowHeight
        anchors.horizontalCenter: wifiConnectBackground.horizontalCenter
        width: 800
        height: 200


        property string inputPassword:""


        color: "white"
        border.color: "black"
        radius: 50

        MouseArea
        {
            anchors.fill: parent
            onClicked:
            {
                inputPanel.visible = false;
            }
        }

        Text {
            id: wifiNameText

            property string wifiName: ""
            text: qsTr(wifiName)
            font.pointSize: 17
            anchors.left: wifiMessageRectangle.left
            anchors.leftMargin: 240
            anchors.top: wifiMessageRectangle.top
            anchors.topMargin: 35
        }

        Text {
            id: wifiPasswordText
            text: qsTr("密码:")
            font.pointSize: 17
            anchors.left: wifiMessageRectangle.left
            anchors.leftMargin: 240
            anchors.top: wifiNameText.top
            anchors.topMargin: 60
        }
        //wifi密码输入框
        Rectangle
        {
            id:inputWifiPassword
    //        width:wifiPasswordTextInput.contentWidth<240?240:wifiPasswordTextInput.contentWidth+10;
            width: 240
            height:40
            color: "lightgray"
            anchors.left: wifiPasswordText.right
            anchors.leftMargin: 10
            anchors.top: wifiNameText.top
            anchors.topMargin: 60
            border.color: "gray"
            TextField
            {
                id:wifiPasswordTextInput
                anchors.fill:parent
                anchors.verticalCenter: parent.verticalCenter
                font.pointSize: 17

                echoMode:TextInput.Password

                onPressed:
                {
                    inputPanel.visible = true;
                }
            }
            Image {
                id: showPasswordImg
                source: "qrc:/img/img/eye_close.png"

                anchors.left: wifiPasswordTextInput.right
                anchors.leftMargin: 10
                width: 40
                height: 40
                MouseArea
                {
                    property bool isShowPassword: false
                    anchors.fill: parent
                    onClicked:
                    {
                        if(!isShowPassword)
                        {
                            wifiPasswordTextInput.echoMode=TextInput.Normal;
                            isShowPassword = true;
                            showPasswordImg.source = "qrc:/img/img/eye.png"
                        }
                        else
                        {
                            wifiPasswordTextInput.echoMode=TextInput.Password;
                            isShowPassword = false;
                            showPasswordImg.source = "qrc:/img/img/eye_close.png"
                        }
                    }
                }

            }
        }

        Text {
            id: connectResult
            anchors.top:inputWifiPassword.bottom
            anchors.topMargin: 10
            anchors.left: inputWifiPassword.left
            font.pointSize: 15
            color: "red"
            text: wifiConnectBackground.connectText
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        //连接wifi按钮
        Rectangle
        {
            anchors.top: inputWifiPassword.bottom
            anchors.topMargin: 20
            anchors.right: parent.right
            anchors.rightMargin: 240
            border.width: 1
            border.color: "grey"
            width: 70
            height: 30
            radius: 15
            Text {
                id: connectWifiText

                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 17
                text: qsTr("加入")
                color: "#007aff"
            }
            MouseArea
            {
                id:wifiConnectSignalSend

                anchors.fill:parent
                onClicked:
                {
                    inputPanel.visible = false;
                    wifiConnectBackground.connectText = "正在连接";
                    //点击后发送连接wifi的信号
                    wifiConnectBackground.connectWifi(wifiNameText.wifiName, wifiPasswordTextInput.text);
                }
            }
        }
    }
}


