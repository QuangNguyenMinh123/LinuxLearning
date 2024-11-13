import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Signal Handler")
    Rectangle{
        id: recId
        width: 100
        height: 100
        color: "red"
        MouseArea{
            anchors.fill: parent
            /* Trigger onEntered handler event without clicking */
            hoverEnabled: true
            /* on clicked handler */
            onClicked: {
                console.log("Red rec is clicked")
            }
            onDoubleClicked: {
                console.log("Red rec is double clicked")
            }
            onEntered: {
                console.log("Red rec is entered")
            }
            onExited: {
                console.log("Red rec is out")
            }
            onWheel: function(wheel){
                console.log("Wheel x:" + wheel.x + "Wheel y:" + wheel.y)
            }
            onPressAndHold: {
                console.log("Was held:" + mouse.wasHeld)
            }
            onPositionChanged: {
                console.log("mouse x:" + mouse.x + "mouse y:" + mouse.y)
            }
        }
    }
}
