import QtQuick 2.0
//显示设备状态的页面

Item {
    property alias propertyNumberValue: statusPanel.numberValue //仪表盘属性的数值
    property alias panelColor: statusPanel.haloColor //仪表板圆环颜色
    property alias maxValue: statusPanel.maxValue //仪表盘指定的最大数值
    property alias propertyName: propertyName.typeName //仪表盘属性的名称
    property alias size:statusHalo.size//整个控件与圆环的大小
    property alias unit:propertyValue.unit


    Rectangle{
        id:statusHalo
        property int size: 0
        width:size
        height: size

        Canvas {
            id: statusPanel
            anchors.centerIn: parent
            width:size
            height: size

            property double numberValue     //当前数值
            property string haloColor       //环的颜色
            property double maxValue        //最大数值
            property int circlex: width/2    //圆心坐标
            property int circley: height/2    //圆心坐标
            property double rotateValue: 1    //旋转的范围
            property bool firstDraw: true

            onPaint: {
                var ctx = getContext("2d");
                ctx.translate(circlex, circley);        //平移
                ctx.rotate(rotateValue*Math.PI);      //正方向旋转270度

                ctx.lineWidth = 8;
                ctx.lineCap = 'round';

                ctx.strokeStyle = "black";
                ctx.beginPath();
                ctx.arc(0, 0, statusPanel.width/2-20, 0, (numberValue/maxValue)*2*Math.PI);  //将要绘制一个圆形
                ctx.stroke();

                ctx.globalCompositeOperation = "source-atop";

                ctx.beginPath();
                ctx.arc(0, 0, statusPanel.width/2-20, 0, 2*Math.PI);  //将要绘制一个圆形
                var gnt1 = ctx.createLinearGradient(60, 0, 0, -60);
                gnt1.addColorStop(0,"#3f4d68");
                gnt1.addColorStop(1,"#578fcc");
                ctx.strokeStyle = gnt1;
                ctx.stroke();

                ctx.globalCompositeOperation = "source-over";
                ctx.fillStyle = "white";
                ctx.strokeStyle = "#4d91f2";
                ctx.lineWidth = 2;
                ctx.beginPath();
                ctx.arc(60*Math.cos((numberValue/maxValue)*2*Math.PI), 60*Math.sin((numberValue/maxValue)*2*Math.PI), 8, 0, 2*Math.PI);
                ctx.fill();
                ctx.stroke();

                ctx.globalCompositeOperation = "destination-over";

                ctx.fillStyle = "white"
                ctx.beginPath();
                ctx.arc(0, 0, statusPanel.width/2-30, 0, 2*Math.PI);  //将要绘制一个圆形
                ctx.fill();

                ctx.fillStyle = "#e6f0fa"
                ctx.lineWidth = 1;
                ctx.beginPath();
                ctx.arc(0, 0, statusPanel.width/2-10, 0, 2*Math.PI);  //将要绘制一个圆形
                ctx.fill();
            }

            //显示当前的数值
            Text {
                anchors.horizontalCenter:parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: -12
                id: propertyValue
                property var unit
                text: statusPanel.numberValue + unit
                font.pixelSize: 28
            }

            //属性名
            Text
            {
                anchors.horizontalCenter:parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: 12
                id: propertyName
                property var typeName   //属性名
                text: qsTr(typeName)
                font.pixelSize: 16
                color: "grey"
            }
        }
    }

//    function canvasRepaint()
//    {
//        statusPanel.circlex=0;
//        statusPanel.circley=0;
//        statusPanel.rotateValue = 0;
//        statusPanel.firstDraw = false;
//        statusPanel.requestPaint();
//    }
}
