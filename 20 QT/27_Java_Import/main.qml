import QtQuick 2.12
import QtQuick.Window 2.12
import "Test_import.js" as Test_import

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("JavaScript Direct Import")

    Rectangle {
        width: 300
        height: 100
        color: "yellowgreen"
        anchors.centerIn: parent

        Text {
            text: "Click Me"
            anchors.centerIn: parent
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                Test_import.greeting();
            }
        }
    }
}
