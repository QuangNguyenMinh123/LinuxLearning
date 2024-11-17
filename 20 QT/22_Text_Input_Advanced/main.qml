import QtQuick 2.12
import QtQuick.Layouts 1.0
import QtQuick.Window 2.12

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("TextInput: Doing Better!")


        LabeledTextInput {
            id: firstNameId
            label: "First Name: "
            placeHolderText: "Type in your first name..."
            onEditingFinished: {
                console.log("The first name changed to: " + firstNameId.text);
            }
        }

        LabeledTextInput {
            id: lastNameId
            x: 0
            y: 100
            label: "Last Name: "
            placeHolderText: "Type in your last name..."
            onEditingFinished: {
                console.log("The last name changed to: " + lastNameId.text);
            }
        }
}
