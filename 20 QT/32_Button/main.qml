import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 1.0

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Button")

    ColumnLayout{
        anchors.left: parent.left
        anchors.right: parent.right

        Button{
            id: button1
            text: "Button1"
            Layout.fillWidth: true
            onClicked: function(){
                console.log("Clicked on button1")
            }
        }

        Button{
            id: button2
            text: "Button2"
            Layout.fillWidth: true
            onClicked: function(){
                console.log("Clicked on button2")
            }
        }
    }
}
