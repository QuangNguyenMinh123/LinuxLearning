import QtQuick 2.12
import QtQuick.Window 2.12
import "test_java.mjs" as Test

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")
    Rectangle{
        width: 100
        height: 100
        color: "yellowgreen"
        anchors.centerIn: parent
        Text {
            text: "Click me"
            anchors.centerIn: parent
        }
        MouseArea
        {
            anchors.fill: parent
            onClicked: {
                console.log("The age is: " + Test.subAge(10,20))
            }
        }
    }
}
