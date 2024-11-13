import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Custom Components in Place")

    Component {
        id: mButtonComponent
        Rectangle {
            id: mButtonId
            property alias buttonText: buttonTextId.text
            signal buttonClicked

            width: buttonTextId.implicitWidth + 20
            height: buttonTextId.implicitHeight + 20
            color: "red"
            border {
                color: "blue"
                width: 3
            }
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
    }
    Column {
    spacing: 20
    width: parent.width
        Component.onCompleted: {
            var button1 = mButtonComponent.createObject(parent);
            button1.buttonText = "Button 3";
            button1.onButtonClicked.connect(function() {
                console.log("Clicked on button 3");
            });
            button1.parent = parent;

            var button2 = mButtonComponent.createObject(parent);
            button2.buttonText = "Button 4";
            button2.onButtonClicked.connect(function() {
                console.log("Clicked on button 4");
            });
            button2.parent = parent;
        }
    }
}
