import QtQuick 2.0

Item {
    id:background
    signal closeAdjustPlaneWindow();
    signal sendCmd(string cmd);
    signal newPosition(string position);

    Rectangle
    {
        anchors.fill: parent
        color: "#7F2F4F4F"
        MouseArea
        {
//            anchors.fill: parent
            height: 680
            width: 1920
            anchors.top: parent.top
            anchors.left: parent.left
            onClicked:
            {
                applicationLoader.source = "";
            }
        }
        Rectangle
        {
            id:operationArea
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            height: 400

            Row
            {
                id:planeFunctionRow
                anchors.left: parent.left
                anchors.leftMargin: 70
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 70
                width: 1800
                spacing: 20
                Repeater
                {
                    model:[
                        {"show":"降至底部"},
                        {"show":"上升0.1"},
                        {"show":"上升0.5"},
                        {"show":"上升10"},
                        {"show":"升至顶部"}
                    ]
                    Rectangle
                    {
                        width:340
                        height:140
                        border.width:1
                        MouseArea
                        {
                            anchors.fill: parent
                            onClicked:
                            {
                                var dataContent = modelData.show;
                                console.log("点击按钮："+dataContent);
                                if(dataContent === "降至底部")
                                {
                                    console.log("发送指令：G28");
                                    background.sendCmd("G28");
                                }
                                if(dataContent === "上升0.1")
                                {
                                    background.sendCmd("G1 Z0.1 F250");
                                }
                                if(dataContent === "上升0.5")
                                {
                                    background.sendCmd("G1 Z0.5 F250");
                                }
                                if(dataContent === "上升10")
                                {
                                    background.sendCmd("G1 Z10 F250");
                                }
                                if(dataContent === "升至顶部")
                                {
                                    background.sendCmd("M18");
                                }
                            }
                        }
                        Text {
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            text: qsTr(modelData.show)
                            font.pointSize: 36
                        }
                    }
                }
            }
            Text {
                id: currentPosition
                anchors.top: parent.top
                anchors.topMargin: 70
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.horizontalCenterOffset: -75
                text: qsTr("当前位置：")
                font.pointSize: 36
            }
            Text {
                id: positionValue
                anchors.left: currentPosition.right
                anchors.leftMargin: 10
                anchors.bottom: currentPosition.bottom
                text: qsTr("120.00")
                font.pointSize: 36
            }
        }
    }
    Connections
    {
        target: background
        function onNewPosition(position)
        {
            positionValue.text = position;
        }
    }
}
