import QtQuick 2.0

/* Build a custom button */
Item{
    id: rootId
    property alias buttonText: buttonTextId.text
    width: containerRectId.width
    height: containerRectId.height
    signal butClicked
    Rectangle{
        id: containerRectId
        width: buttonTextId.implicitWidth + 10
        height: buttonTextId.implicitHeight + 10
        color: "red"
        border{color: "black"; width: 3}
        Text{
            id: buttonTextId
            text: "BUTTON"
            anchors.centerIn: parent
        }
        MouseArea{
            anchors.fill: parent
            onClicked:
            {
                rootId.butClicked()
            }
        }
    }
}
