import QtQuick 2.0
import material.value 1.0
import QtQuick.Controls 2.0

Item {
    Rectangle{
        id:materialPage
        anchors.top:parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 60
        color: "#f2f2f7"
        //ListView主体
        ListView{
            id:materialValueList
            anchors.fill: parent
            anchors.topMargin: 10
            anchors.bottomMargin: 10
            anchors.leftMargin: 30
            anchors.rightMargin: 30
            delegate:showSignalMaterialValue
            model:MaterialModel{id:materialList}
            clip: true
        }
        //ListView内每一行的信息
        Component{
            id:showSignalMaterialValue
            Rectangle{
                id:materialValueItem
                width: materialValueList.width
                height: 55
                color: materialValueItem.ListView.isCurrentItem ? "#007aff":"white"//判断自己是否被选中
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        materialValueItem.ListView.view.currentIndex = index;
                    }
                }
                Text {
                    id: signalMaterialName
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    color: materialValueItem.ListView.isCurrentItem ? "white":"grey"
                    font.pointSize: 20
                    text: materialName
                }
                Text {
                    id: signalMaterialLight
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 30
                    color: materialValueItem.ListView.isCurrentItem ? "white":"grey"
                    font.pointSize: 20
                    text: materialLight
                }
                //绘制分隔线
                Canvas{
                    id:materialDrawArea
                    anchors.bottom: parent.bottom

                    width:materialValueList.width
                    height: 1
                    onPaint: {
                        var ctx = getContext("2d");
                        ctx.lineWidth = 1;
                        ctx.strokeStyle = "#c6c6c8";
                        ctx.beginPath();
                        ctx.moveTo(50,1);
                        ctx.lineTo(width,1);
                        //保存行程
                        ctx.stroke();
                    }
                }
            }
        }
        //材料同步按钮
        /*
        Rectangle
        {
            id:materialSyncBottom
            anchors.top: materialPage.bottom
            anchors.topMargin: 20
            anchors.right: parent.right
            anchors.rightMargin: 40
            width: 120
            height: 40
            radius: 10
            color: "white"
            border.width:1
            border.color: "black"
            MouseArea
            {
               anchors.fill: parent
               onClicked:
               {
                   materialList.startSync();
                   materialList.updateMaterial();
               }
            }
            Text {
               id: materialSync
               anchors.horizontalCenter: parent.horizontalCenter
               anchors.verticalCenter: parent.verticalCenter
               text: qsTr("材料同步")
               horizontalAlignment: Text.AlignHCenter
               verticalAlignment: Text.AlignVCenter
               font.pointSize: 12
            }
        }
        */
    }
}
