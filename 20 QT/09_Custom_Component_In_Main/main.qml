import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Custom Components in Place")
    property var aFont: Qt.font({family: "ubuntu", pointSize: 30, bold: true})

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
            font: aFont
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
        MButton {
            buttonText: "234"
            onButtonClicked: {
                console.log("Clicked on inlined component button3");
            }
        }
        MButton {
            buttonText: "123"
            onButtonClicked: {
                console.log("clicked on inlined component button4");
            }
        }
    }

}
