import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")
    Rectangle{
        width: 100
        height: 100
        id: recId
        color: "beige"
        /* Setup signal */
        signal source_signal(string msg1)
        signal des_signal(string msg2)
        /*  */
        function respond_your_way(msg)
        {
            console.log("Custom respond: " + msg)
        }
        MouseArea{
            width: 100
            height: 100
            anchors.fill: parent
            onClicked: {
                recId.des_signal("des_signal")
                recId.source_signal("source_signal")
            }
        }
    }
    Component.onCompleted: {
        /* Connect signal to signal */
        recId.source_signal.connect(recId.des_signal)
        /* Connect destination signal to final handler */
        recId.des_signal.connect(recId.respond_your_way)
    }
}
