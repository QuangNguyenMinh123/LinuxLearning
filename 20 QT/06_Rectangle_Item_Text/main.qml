import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    property var aFont: Qt.font({family: "Consolas", pointSize: 30, bold: true})
    visible: true
    width: 640
    height: 480
    title: qsTr("QML Basic Element")
    Item{
        id: containerId
        x: 50
        y: 50
        width: 400
        height: 200
        Rectangle{
            anchors.fill: parent
            color: "beige"
            border { color: "black"; width: 5 }
            radius: 10
        }
        Rectangle{
            x : 5; y : 5; width: 50; height: 50;
            color: "green"
        }
        Text{
            anchors.centerIn: parent
            color: "black"
            font: aFont
            text: "Hello World"
        }
    }
}
