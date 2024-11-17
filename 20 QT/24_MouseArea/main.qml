import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")
    Rectangle{
        id: containerRecId
        width: parent.width
        height: 200
        color: "beige"
        Rectangle{
            id: movingId
            width: 50
            height: width
            color: "green"
        }
        MouseArea{
            anchors.fill: parent
            hoverEnabled: true
            onClicked: function(mouse)
            {
                console.log(mouse.x)
                movingId.x = mouse.x
            }
            onPositionChanged: function(mouse){
                movingId.x = mouse.x
            }
            onWheel: mouse=>{
                console.log("wheel: " + mouse.angleDelta)
            }
            onHoveredChanged: {
                if (containsMouse)
                    containerRecId.color = "yellow"
                else
                    containerRecId.color = "red"
            }
        }
    }
    /* below rec */
    Rectangle{
        id: dragContainerId
        anchors.bottom: parent.bottom
        width: parent.width
        height: 200
        color: "beige"
        Rectangle{
            id: dragRecId
            width: 50
            height: width
            color: "green"
            onXChanged: {
                console.log("x is changed to: " + x)
            }
        }
        MouseArea{
            anchors.fill: parent
            drag.target: dragRecId
            drag.axis: Drag.XAndYAxis
            drag.minimumX: 0
            drag.maximumX: dragContainerId.width - dragRecId.width
        }
    }
}
