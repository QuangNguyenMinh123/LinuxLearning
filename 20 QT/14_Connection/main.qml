import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Connections")

    Rectangle {
        id: rectId
        width: 200
        height: 200
        color: "dodgerblue"

        MouseArea {
            id: mouseAreaId
            anchors.fill: parent
        }
    }
    Connections {
        target: mouseAreaIdo
        onClicked: {
            console.log("Clicked on the mouse area")
        }
        onDoubleClicked: {
            console.log("Double clicked at: " + mouse.x + ", " + mouse.y)
        }
    }
}
