import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")
    Rectangle{
        id: recId
        width: 100
        height: 100
        color: "red"
        signal somesignal(string msg)
        onSomesignal: msg => {
            console.log("String is: " + msg)
        }
        MouseArea{
            anchors.fill: parent
            onClicked: {
                recId.somesignal("ARBITRARY STRING")
            }
        }
    }
}
