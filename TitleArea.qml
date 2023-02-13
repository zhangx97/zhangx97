import QtQuick 2.0
//标题

Item{
    Rectangle{
        radius: 90
        color: "white"
        anchors.fill: parent


        Image {
            width: 100
            height: 100
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            id: logo
            source: "qrc:/img/img/logo.png"
        }

        Item{
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.rightMargin: 5
            anchors.bottom: parent.bottom
            anchors.left: logo.right


//            width:110

            Text {
                id: clinicName
                property string textClinicName:"筑真3D打印机"

                text: qsTr(textClinicName)  //接收C++后端发来的诊所名称
                anchors.centerIn: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter

                font.pointSize: 40
                color: "black"
            }
        }


    }


}
