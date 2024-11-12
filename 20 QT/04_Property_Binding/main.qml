import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")
    Rectangle
    {
        id: redRecId
        width: 20
        height: width * 1.5
        color: "red"
        MouseArea
        {
            anchors.fill: parent
            onClicked:
            {
                console.log("Red rectangle is getting bigger")
                redRecId.width += 10
            }
        }
    }
    Rectangle
    {
        id: greenRecId
        width: 20
        height: width * 2
        color: "green"
        anchors.bottom: parent.bottom
        MouseArea
        {
            anchors.fill: parent
            onClicked:
            {
                console.log("green is getting bigger")
                greenRecId.width += 10
            }
        }
    }
    Rectangle
    {
        id: blueRecId
        width: 20
        height: width * 2
        color: "blue"
        anchors.bottom: parent.bottom
        anchors.left: greenRecId.right
        MouseArea
        {
            anchors.fill: parent
            onClicked:
            {
                /* redRecId.height is fixed to static value 100 or redRecId.width * 2 and binding is broken
                    (redRecId.height: redRecId.width * 1.5) */
//                redRecId.height = 100
//                redRecId.height = redRecId.width * 2
                /* To re-Binding*/
                redRecId.height = Qt.binding(function(){
                    return redRecId.width * 1
                })
            }
        }
    }
}
