import QtQuick 2.0
import QtQuick.Controls 1.4
import logControl 1.0

Rectangle
{
    id:logBackground
    anchors.fill: parent
    height: 1080
    width: 1920
    color:"#7F2F4F4F"

    signal closeLogWindow();
    MouseArea
    {
        anchors.fill: parent
        onClicked:
        {
            logBackground.closeLogWindow();
        }
    }
    ShowLog
    {
        id:logShow
    }
    Rectangle//界面
    {
        id:logMainWindow
        anchors.horizontalCenter:parent.horizontalCenter
        anchors.verticalCenter:parent.verticalCenter
        height:768
        width:1366
        MouseArea
        {
            anchors.fill: parent
        }
        Rectangle//显示日历的按钮
        {
            id:showCalendar
            property bool clickFlag: false
            anchors.top:parent.top
            anchors.topMargin:30
            anchors.left:parent.left
            anchors.leftMargin:60
            width:120
            height:40
            color:"grey"
            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                font.pointSize: 15
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: qsTr("选择日期")
            }

            MouseArea
            {
                anchors.fill: parent
                onClicked:
                {
                    if(!showCalendar.clickFlag)
                    {
                        logTimeChose.visible = true;
                        showCalendar.clickFlag = true;
                    }
                    else
                    {
                        logTimeChose.visible = false;
                        showCalendar.clickFlag = false;
                    }

                }
            }

        }
        TableView
        {
            id:logContent
            anchors.top: showCalendar.bottom
            anchors.topMargin: 20
            anchors.left: parent.left
            anchors.leftMargin: 60
            anchors.right: parent.right
            anchors.rightMargin: 60
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 60
            TableViewColumn{ role: "time"  ; title: "时间" ; width: 150; elideMode: Text.ElideLeft;}
            TableViewColumn{ role: "level" ; title: "等级" ; width: 60; }
            TableViewColumn{ role: "content" ; title: "内容" ; width: 600; elideMode:Text.ElideRight;}
            TableViewColumn{ role: "position" ; title: "位置" ; width: 200; elideMode:Text.ElideLeft;}
            TableViewColumn{ role: "function" ; title: "方法" ; width: 140; elideMode:Text.ElideLeft;}
            TableViewColumn{ role: "row" ; title: "行数" ; width: 60;}
            model:logShow
        }
        Calendar//日历
        {
            id:logTimeChose
            anchors.top:showCalendar.bottom
            anchors.left:showCalendar.left
            width:300
            height:300
            visible:false
            onClicked: {
                logShow.choseDate = Qt.formatDateTime(logTimeChose.selectedDate, "yyyyMMdd");
                logShow.refresh();
                showCalendar.clickFlag = false;
                logTimeChose.visible = false;
            }
        }
        Rectangle//前一天按钮
        {
            id:theDayBefore
            MouseArea
            {
                anchors.fill: parent
            }
        }
        Rectangle//后一天按钮
        {
            id:theDayAfter
            MouseArea
            {
                anchors.fill: parent
            }
        }
    }
}
