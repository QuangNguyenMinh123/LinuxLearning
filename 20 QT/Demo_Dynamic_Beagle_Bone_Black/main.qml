import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    visible: true
    width: 1280
    height: 1024
    title: qsTr("Hello World")
    color: "lightblue"
    Rectangle{
        id: recID
        x: 0
        y: 0
        width: 50
        height: 50
        color: "blue"
        focus: true     /* Important if u want to handle key events */
        Behavior on x { NumberAnimation { duration: 1 }}
        Behavior on y { NumberAnimation { duration: 1 }}
        Keys.onRightPressed: function(event)
        {
            recID.x += 5
        }
        Keys.onLeftPressed: function(event)
        {
            recID.x -= 5
        }
        Keys.onUpPressed: function(event)
        {
            recID.y -= 5
        }
        Keys.onDownPressed: function(event)
        {
            recID.y += 5
        }
    }
}
