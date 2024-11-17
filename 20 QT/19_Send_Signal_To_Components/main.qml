import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")
    Notifier{
        id: notifierId
        target: receiverId
        rectColor: "green"
        onNotify: count => {
            console.log("Notify: " + count)
        }
    }
    Receiver{
        anchors.right: parent.right
        id: receiverId
        rectColor: "red"

    }
//    Component.onCompleted: {
//        notifierId.notify.connect(receiverId.receiveInfo)
//    }
}
