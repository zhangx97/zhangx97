import QtQuick 2.0
//显示状态的区域
Item {
    property alias machineStatusText: statusPanel.textMachineStatus
    property alias slicepackInfo: statusPanel.packInfo
    property alias serialNumber: statusPanel.printerNumber

    property alias statusLight: statusPanel.machineLight
    property alias statusTemp: statusPanel.machineTemp
    property alias statusCurrent: statusPanel.machineCurrent
    property alias statusResinTemp: statusPanel.resinTemp

    //显示状态文字
    Rectangle{
        id:statusPanel
        anchors.fill:parent
        border.color: "white"
        color:"white"
        radius:30
        border.width: 8
        property string textMachineStatus: "空闲"
        property var packInfo: []//slicepack内的信息
        property string printerNumber: ""//序列号

        property int machineLight: 0//光机亮度
        property double machineTemp: 0.0//光机温度
        property int machineCurrent: 0//光机电流
        property double resinTemp: 0.0//树脂温度

        //圆点
        Rectangle{
            anchors.left: parent.left
            anchors.leftMargin: 25
            anchors.top:parent.top
            anchors.topMargin: 25
            width: 16
            height: 16
            radius: 8
            color: "#34c759"
        }

        //右上角机器状态的文字
        Text {
            id: machineStatus
            text: qsTr(statusPanel.textMachineStatus)    //机器的状态：C++后端发送数据到这里
            color: "#34c759"
            font.pointSize: 30

            anchors.left: parent.left
            anchors.leftMargin: 45
            anchors.top:parent.top
            anchors.topMargin: 10

            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }

        //系统的三个圆环
        Row
        {
            id:panelRow
            anchors.top: parent.top
            anchors.topMargin: 60
            anchors.left: parent.left
            anchors.leftMargin: 20
            anchors.right: parent.right
            spacing: 5

            Repeater
            {
                model: [
                    {"name":"电流","color":"#34c759","value":statusPanel.machineCurrent,"maxValue":2.0},
                    {"name":"亮度","color":"#007aff","value":statusPanel.machineLight,"maxValue":1024},
                    {"name":"温度","color":"#ff3b30","value":statusPanel.machineTemp,"maxValue":120},
                    {"name":"树脂温度","color":"#F83E05","value":statusPanel.resinTemp,"maxValue":120}
                ]
                Rectangle
                {
                    width: 150
                    height: 150
                    MachineStatusPanel{
                        propertyName:modelData.name
                        propertyNumberValue: modelData.value
                        panelColor:modelData.color
                        maxValue:modelData.maxValue
                        size:100
                    }
                }
            }
        }

        //序列号
        Rectangle
        {
            id: printSerialNum
            anchors.top: panelRow.bottom
            anchors.topMargin: 80
            anchors.left: parent.left
            anchors.right: parent.right
            color: "#F5F5F5"
            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                text: statusPanel.printerNumber
                font.pointSize: 20
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }

        //材料型号,切层厚度,固化时间
        Rectangle
        {
            id:sliceInfoRow
            anchors.top: printSerialNum.bottom
            anchors.topMargin: 5
            Row
            {
                anchors.top: parent.top
                anchors.topMargin: 60
                anchors.left: parent.left
                anchors.leftMargin: 20
                anchors.right: parent.right
                spacing: 30
                Repeater
                {
                    model: [
                        {"name":"材料型号","property":statusPanel.packInfo[0]},
                        {"name":"切层厚度","property":statusPanel.packInfo[1]},
                        {"name":"固化时间","property":statusPanel.packInfo[2]}
                    ]
                    Rectangle
                    {
                        width: 180
                        height: 120
                        color: "#F5F5F5"
//                        border.width: 1
//                        border.color: "black"
                        radius: 10
                        Text {
                            anchors.top: parent.top
                            anchors.topMargin: 2
                            anchors.left:parent.left
                            anchors.leftMargin: 4
//                            id: name
                            text: modelData.name
                            font.pointSize: 12
                        }
                        Text {
//                            id: name
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                            text: modelData.property
                            font.pointSize: 18
                        }
                    }
                }
            }
        }
    }
}
