import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("QML Syntax demo")
    property string textToShow: "HELLO"
    Rectangle
    {
        id: redRecId
        x: 10
        y: 10
        width: 100
        height: 100
        color: "red"
    }
    Rectangle
    {
        id: blueRecId
        x:  redRecId.x + redRecId.width
        y:  redRecId.y + redRecId.height
        width: 100
        height: 100
        color: "blue"
        radius: 30
    }
    Rectangle
    {
        id: centerId
        anchors.centerIn: blueRecId
        width: 50
        height: 50
        color: "green"
        Text {
            id: textId
            anchors.centerIn: parent
            text: textToShow
        }
        MouseArea
        {
            anchors.fill: parent    /* To tell MouseArea to fill the rectangle that contains it */
            onClicked: {
                console.log("Clicked on the large rectangle")
                textId.text = "Clicked on the large rectangle"
            }
        }
    }
    Row
    {
        x: 200
        y: 200
        Rectangle {
            width: 50; height: 50; color: "red"; border.color: "black"; border.width: 10; radius: 5

            MouseArea{
                anchors.fill: parent
                onClicked: {
                    console.log("Clicked on the red rectangle")
                    textId.text = "red"
                }
            }
        }
        Rectangle {
            width: 50; height: 50; color: "green"; border.color: "yellow"; border.width: 15; radius: 5
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    console.log("Clicked on the green rectangle")
                    textId.text = "green"
                }
            }
        }
        Rectangle {
            width: 50; height: 50; color: "blue"; border.color: "pink"; border.width: 20; radius: 5
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    console.log("Clicked on the blue rectangle")
                    textId.text = "blue"
                }
            }
        }
        Rectangle {
            width: 50; height: 50; color: "black"; border.color: "red"; border.width: 5; radius: 60
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    console.log("Clicked on the black rectangle")
                    textId.text = "black"
                }
            }
        }
    }
}
