import QtQuick 2.12
import QtQuick.Controls 2.12

ApplicationWindow {
    visible: true
    width: 640
    height: 480

    title: "Hello, BeagleBone Black!"

    Rectangle {
        width: parent.width
        height: parent.height

        color: "lightblue"

        Text {
            id: greetingText
            anchors.centerIn: parent
            text: "Hello, BeagleBone Black!"
            font.pixelSize: 32
        }
    }
}
