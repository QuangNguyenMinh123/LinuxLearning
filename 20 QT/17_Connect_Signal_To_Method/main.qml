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
        color: "beige"
        /* Setup the signal */
        signal somesignal(string msg)
        /* Set up custom handler */
        function respond_your_way(msg)
        {
            console.log("This is string: " + msg)
        }
    }
    Component.onCompleted: {
        /* Connect signal to handler */
        recId.somesignal.connect(recId.respond_your_way)
    }
    MouseArea{
        anchors.fill: parent
        onClicked: {
            recId.somesignal("ARBITRARY STRING")
        }
    }
}
