import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")
    Rectangle{
        id: recId
        width: 100
        height: 100
        color: "green"
        MouseArea{
            anchors.fill: parent
            hoverEnabled: true
//            onClicked: {
//                console.log("x: " + mouse.x + " y: " + mouse.y)
//            }
//            onClicked: function(mouse_para){
//                console.log("x: " + mouse_para.x + " y: " + mouse_para.y)
//            }
            onClicked: arbitrary => {
                console.log("x: " + arbitrary.x + " y: " + arbitrary.y)
            }
        }
    }
}
