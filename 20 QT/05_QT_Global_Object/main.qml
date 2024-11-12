import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    id: rootId
    visible: true
    width: 640
    height: 480
    title: qsTr("Qt Global object")
    property string abc: "My String"
    property var fonts: Qt.fontFamilies()
    onAbcChanged:{
        console.log("Name has changed")
    }
    onTitleChanged: console.log("Title has changed")
    Text {
        id: arbitraryText
        text: qsTr("THIS IS ARBITRARY TEXT")
        anchors.centerIn: parent
        font.pointSize: 20
        color: Qt.rgba(1, 0, 0, 1)
    }
    Rectangle{
        width: 50
        height: 50
        color: "red"
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        Text {
            text: qsTr("QUIT")
            anchors.centerIn: parent
            font.pointSize: 10
            color: Qt.rgba(0, 1, 1, 1)
            font.bold: true
        }
        MouseArea
        {
            anchors.fill: parent
            onClicked:
                Qt.quit()
        }

    }
    Rectangle{
        id: greenId
        width: 100
        height: 50
        color: "green"
        anchors.bottom: parent.bottom
        Text {
            text: qsTr("Open README")
            anchors.centerIn: parent
            font.pointSize: 10
            color: Qt.rgba(0, 1, 1, 1)
            font.bold: true
        }
        MouseArea
        {
            anchors.fill: parent
            onClicked:
            {
                Qt.openUrlExternally("main.cpp")
            }
        }

    }
    Rectangle{
        id: checkOsId
        width: 100
        height: 50
        color: "green"
        anchors.bottom: parent.bottom
        anchors.left: greenId.right
        Text {
            text: qsTr("Check OS")
            anchors.centerIn: parent
            font.pointSize: 10
            color: Qt.rgba(0, 1, 1, 1)
            font.bold: true
        }
        MouseArea
        {
            anchors.fill: parent
            onClicked:
            {
                console.log("OS: " + Qt.platform.os)
            }
        }
    }
    Rectangle{
        width: 120
        height: 50
        color: "black"
        anchors.bottom: parent.bottom
        anchors.left: checkOsId.right
        Text {
            text: qsTr("Change title/name")
            anchors.centerIn: parent
            font.pointSize: 10
            color: Qt.rgba(0, 1, 1, 1)
            font.bold: true
        }
        MouseArea
        {
            anchors.fill: parent
            onClicked:
            {
                rootId.abc = "New name"
                rootId.title = "King of the sky"
            }
        }
    }
}
