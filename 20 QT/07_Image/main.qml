import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    id: rootID
    visible: true
    width: 640
    height: 480
    title: qsTr("Image")
    Item{
        id: itemContainerId
        x: 50
        y: 50
        width: rootID.width - 100
        height: rootID.height - 100
        Rectangle{
            anchors.fill: parent
            color: "beige"
        }
        Image{
            x: 0
            y: 0
            width: 100
            height: 100
            source: "file:/home/quangnm/Documents/GitHub/LinuxLearning/20 QT/QT.png"
        }
        Image{
            x: 120
            y: 0
            width: 100
            height: 100
            source: "qrc:/images/QT.png"
        }
    }
}
