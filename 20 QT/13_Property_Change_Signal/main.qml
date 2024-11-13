import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")
    Rectangle{
        id: recId
        width: 300
        height: width + 30
        color: "beige"
        property string description: "A rectangle"
        property string abc: ""
        /* property change handler */
        onWidthChanged: {
            console.log("Width change to: " + recId.width)
        }
        onHeightChanged: {
            console.log("Height change to: " + recId.height)
        }
        onColorChanged: {}
        onDescriptionChanged: {}
        onAbcChanged: {}
        MouseArea{
            anchors.fill: parent
            onClicked: {
                recId.width += 10
                recId.abc = "abc"
            }
        }
    }
}
