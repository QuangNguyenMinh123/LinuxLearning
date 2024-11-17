import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")
    Row{
        x: 10
        y: 10
        id: firstRow
        spacing: 10
        Rectangle{
            id: firstNameId
            width: firstNameLabelId.implicitWidth + 10
            height: firstNameLabelId.implicitHeight + 10
            color: "beige"
            Text{

                id: firstNameLabelId
                anchors.centerIn: parent
                text: "First Name"
            }
        }
        Rectangle{
            id: firstNameTextRecId
            width: firstNameTextId.implicitWidth + 10
            height: firstNameTextId.implicitHeight + 10
            color: "beige"
            TextInput{
                id: firstNameTextId
                anchors.centerIn: parent
                text: "Type in your firstname"
                onEditingFinished: {
                    console.log("The first name is changed to: " + text)
                }
            }
        }
    }
    /* 2nd row */
    Row{
        x: 10
        y: 10
        anchors.top: firstRow.bottom
        spacing: 10
        Rectangle{
            id: laseNameId
            width: lastNameLabelId.implicitWidth + 10
            height: lastNameLabelId.implicitHeight + 10
            color: "beige"
            Text{

                id: lastNameLabelId
                anchors.centerIn: parent
                text: "Last Name"
            }
        }
        Rectangle{
            id: laseNameTextRecId
            width: laseNameTextId.implicitWidth + 10
            height: laseNameTextId.implicitHeight + 10
            color: "beige"
            TextInput{
                id: laseNameTextId
                anchors.centerIn: parent
                text: "Type in your lastname"
                onEditingFinished: {
                    console.log("The last name is changed to: " + text)
                }
            }
        }
    }
}
