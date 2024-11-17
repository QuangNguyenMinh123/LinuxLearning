import QtQuick 2.12
import QtQuick.Window 2.12

Item {
    property alias rectColor: receiverId.color
    width: receiverId.width
    height: receiverId.height
    property int count: 0
    function receiveInfo(cnt){
        displayTextId.text = cnt
    }

    Rectangle{
        id: receiverId
        width: 200
        height: 200
        color: "green"
        Text{
            text: "Receiver"
        }
        Text{
            id: displayTextId
            anchors.centerIn: parent
            text: "0"
        }
        MouseArea{
            anchors.fill: parent
            onClicked: {

            }
        }
    }
}
