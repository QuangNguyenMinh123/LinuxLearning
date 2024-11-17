import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")
    /* Signal with 3 parameters */
    signal info(string last_name, string first_name, string age)
    /* Handle all 3 parameters */
    onInfo: function(l,f,a){
        print("lase name: " + l + " first name: " + f + " age: " + a)
    }
    Rectangle{
        width: 50
        height: 50
        color: "red"
        MouseArea
        {
            anchors.fill: parent
            onClicked:
                info("Snow", "John", 33)
        }
    }
}
