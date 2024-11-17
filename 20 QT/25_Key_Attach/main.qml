import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")
    Rectangle{
        id: containerRectId
        anchors.centerIn: parent
        width: 300
        height: 50
        color: "beige"

        focus: true     /* Important if u want to handle key events */
        Keys.onDigit5Pressed: function(event)
        {
            console.log("key: 5")
            event.accepted = false  /* other event may handler it */
        }
        Keys.onPressed: function(event)
        {
            if ((event.key === Qt.Key_5) && Qt.ControlModifier && event.modifiers) {
                console.log("ctrl 5 is pressed")
            }

        }
    }
}
