import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")
    TextEdit{
        width: 240
        text: "<b>Hello</b> <i>World!</i>"
        font.family: "Helvetica"
        font.pointSize: 20
        color: blue
        focus: true
        wrapMode: TextEdit.Wrap
        textFormat: TextEdit.RichText
        onEditingFinished: {
            console.log("Current text is: " + text)
        }
    }
}
