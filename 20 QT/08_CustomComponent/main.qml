import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Custom Components in Place")

    component MButton: Rectangle{
        id: mButtonId
        property alias buttonText: buttonTextId.text
        signal buttonClicked
        width: buttonTextId.implicitWidth + 20
        height: buttonTextId.implicitHeight + 20
        color: "red"
        border {color: "blue"; width: 3}

        Text {
            id: buttonTextId
            text: "Button"
            anchors.centerIn: parent
            onTextChanged: {
                console.log("Text changed to " + buttonTextId.text);
            }
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                mButtonId.buttonClicked();
            }
        }
    }


    Column {
        spacing: 20
        MButton {
            buttonText: "Button3"
            onButtonClicked: {
                console.log("Clicked on inlined component button3");
            }
        }
        MButton {
            buttonText: "Button4"
            onButtonClicked: {
                console.log("clicked on inlined component button4");
            }
        }
    }
}