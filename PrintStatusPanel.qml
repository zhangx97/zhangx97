import QtQuick 2.0
//显示设备状态的页面

Item {
    property alias propertyNumberValue: printStatusHalo.numberValue //仪表盘属性的数值
    property alias panelColor: statusPanel.haloColor //仪表板圆环颜色
    property alias maxValue: printStatusHalo.maxValue //仪表盘指定的最大数值
    property alias propertyName: printStatusHalo.typeName //仪表盘属性的名称
    property alias size:printStatusHalo.size//整个控件与圆环的大小
    property alias haloPercent: statusPanel.percent//百分比
    property alias percentNumberValue:printStatusHalo.percentNumber

    Item{
        id:printStatusHalo
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        property int size: 0
        property var typeName:""   //属性名
        property double numberValue:0.0     //当前数值
        property double maxValue:0.0        //最大数值
        property string percentNumber: "0%"

        width:size
        height: size//160

        Canvas{
            id: statusPanel
            anchors.top:parent.top
//            anchors.topMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
            width:printStatusHalo.size
            height: printStatusHalo.size
            property double percent:0.0       //百分比
            property string haloColor:"green"       //环的颜色
            property int circlex: width/2    //圆心坐标
            property int circley: height/2    //圆心坐标
            property double rotateValue: 1.5    //旋转的范围
            property bool firstDraw: true

            onPaint: {

                var ctx = getContext("2d");
                ctx.strokeStyle = haloColor;
                ctx.lineWidth = 16;
                ctx.translate(circlex,circley);
                ctx.rotate(rotateValue*Math.PI);
                ctx.beginPath();
                ctx.arc(0, 0, width/2-10, 0, percent*2*Math.PI);  //将要绘制一个圆形
                ctx.stroke();
                if(firstDraw)
                {
                    var actx = getContext("2d");
                    actx.strokeStyle = "grey";
                    actx.lineWidth = 16;
                    actx.translate(0,0);
                    actx.rotate(0*Math.PI);
                    actx.beginPath();
                    actx.arc(0, 0, width/2-10, 0, 2*Math.PI);  //将要绘制一个圆形
                    actx.stroke();
                }
            }
            Text {
                id: propertyValue
                anchors.horizontalCenter:parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                font.pointSize: 16
                anchors.bottomMargin: 20
                text: printStatusHalo.typeName
            }
            Text {
                id: printPercent
                anchors.horizontalCenter:parent.horizontalCenter
                anchors.top:parent.top
                anchors.topMargin: 80
//                anchors.bottom: parent.center
                font.pointSize: 16
                text: printStatusHalo.percentNumber
            }
            function canvasRepaint()
            {
                statusPanel.circlex=0;
                statusPanel.circley=0;
                statusPanel.rotateValue = 0;
                statusPanel.firstDraw = false;
                statusPanel.requestPaint();
//                console.log("界面重绘")
            }
        }

    }
    Text {
        anchors.top: printStatusHalo.bottom
        anchors.topMargin: 10
        anchors.horizontalCenter: printStatusHalo.horizontalCenter
        id: propertyName
        text: printStatusHalo.numberValue+"/"+printStatusHalo.maxValue
        font.pointSize: 16
    }
    function haloRepaint()
    {
        statusPanel.canvasRepaint();
//        propertyName.text =
//        console.log("界面重绘")
    }
}

