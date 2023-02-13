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
            anchors.fill: parent
            onClicked:
            {
                background.closeAdjustPlaneWindow();
            }
        }
        Rectangle
        {
            id:operationArea
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            height: 200

            Row
            {
                id:planeFunctionRow
                anchors.left: parent.left
                anchors.leftMargin: 300
                anchors.verticalCenter: parent.verticalCenter
                width: 1000
                spacing: 15
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
                        width:200
                        height:80
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
                            font.pointSize: 16
                        }
                    }
                }
            }
            Text {
                id: currentPosition
                anchors.left: planeFunctionRow.right
                anchors.leftMargin: 200
                anchors.verticalCenter: parent.verticalCenter
                text: qsTr("当前位置：")
                font.pointSize: 16
            }
            Text {
                id: positionValue
                anchors.left: currentPosition.right
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                text: qsTr("50.00")
                font.pointSize: 16
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
