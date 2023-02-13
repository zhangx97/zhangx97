import QtQuick 2.0
//应用列表，以后实现,现在暂时用直接的图标代替
Item {
    Rectangle{
        anchors.fill: parent
        color: "white"
        radius: 30

        ListView{
            id: listView
            anchors.centerIn: parent
            anchors.fill:parent
            anchors.margins: 20
            model:6     //C++后台把发送来的模型传输到这里
            delegate: application
            clip: true
            spacing:30
            orientation: ListView.Horizontal
        }
        Component{
            id:application
            FunctionBottom{
            }
        }
    }
}
