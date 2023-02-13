import QtQuick 2.0
//显示设备状态的页面

Item {
    property alias propertyNumberValue: statusPanel.numberValue //仪表盘属性的数值
    property alias panelColor: statusPanel.haloColor //仪表板圆环颜色
    property alias maxValue: statusPanel.maxValue //仪表盘指定的最大数值
    property alias propertyName: propertyName.typeName //仪表盘属性的名称
    property alias size:statusHalo.size//整个控件与圆环的大小

    Rectangle{
        id:statusHalo
        property int size: 0
        width:size
        height: size//160
        Canvas{
            id: statusPanel
            anchors.top:parent.top
            anchors.topMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
            width:size
            height: size

            property double numberValue     //当前数值
            property string haloColor       //环的颜色
            property double maxValue        //最大数值
            property int circlex: width/2    //圆心坐标
            property int circley: height/2    //圆心坐标
            property double rotateValue: 1.5    //旋转的范围
            property bool firstDraw: true

            onPaint: {
                var ctx = getContext("2d");
                ctx.strokeStyle = haloColor;
                ctx.lineWidth = 8;
                ctx.translate(circlex, circley);
                ctx.rotate(rotateValue*Math.PI);
                ctx.beginPath();
                ctx.arc(0, 0, statusPanel.width/2-10, 0, (numberValue/maxValue)*2*Math.PI);  //将要绘制一个圆形
                ctx.stroke();
//                if(firstDraw)
//                {
//                    var actx = getContext("2d");
//                    actx.strokeStyle = "#7F2F4F4F";
//                    actx.lineWidth = 8;
//                    actx.translate(0,0);
//                    actx.rotate(0*Math.PI);
//                    actx.beginPath();
//                    actx.arc(0, 0, width/2-10, 0, 2*Math.PI);  //将要绘制一个圆形
//                    actx.stroke();
//                }

            }

            //显示当前的数值
            Text {
                anchors.horizontalCenter:parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                id: propertyValue
                text: statusPanel.numberValue
            }
        }
    }

    //属性名
    Text
    {
        anchors.top: statusHalo.bottom
        anchors.topMargin: 10
        anchors.bottomMargin: 10
        anchors.horizontalCenter: statusHalo.horizontalCenter
        id: propertyName
        property var typeName   //属性名
        text: qsTr(typeName)
        font.pointSize: 16
    }

    function canvasRepaint()
    {
        statusPanel.circlex=0;
        statusPanel.circley=0;
        statusPanel.rotateValue = 0;
        statusPanel.firstDraw = false;
        statusPanel.requestPaint();
    }
}
