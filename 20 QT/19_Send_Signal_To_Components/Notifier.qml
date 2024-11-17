import QtQuick 2.12
import QtQuick.Window 2.12

Item {
    property alias rectColor: notifierRectId.color
    width: notifierRectId.width
    height: notifierRectId.height
    property int count: 0
    signal notify(int cnt)
    property Receiver target
    onTargetChanged: {
        notify.connect(target.receiveInfo)
    }
    Rectangle{
        id: notifierRectId
        width: 200
        height: 200
        color: "red"
        Text{
            text: "Notifier"
        }
        Text{
            id: displayTextId
            anchors.centerIn: parent
            text: count
        }
        MouseArea{
            anchors.fill: parent
            onClicked: {
                count++;
                notify(count)
            }
        }
    }
}
