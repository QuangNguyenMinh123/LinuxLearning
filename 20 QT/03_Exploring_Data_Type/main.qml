import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("QML Data Type Demo")
    property string mString: "https://www.google.com"
    property int mInt: 10
    property bool isFemale: true
    property double mDouble: 10.5
    property url mUrl: "https://www.facebook.com"

    property var aNumber: 100
    property var aBool: false
    property var aString: "Hello world!"
    property var anotherString: String("#FF008800")
    property var aColor: Qt.rgba(0.2, 0.3, 0.4, 0.5)
    property var aRect: Qt.rect(10, 10, 10, 10)
    property var aPoint: Qt.point(10, 10)
    property var aSize: Qt.size(10, 10)
    property var aVector3d: Qt.vector3d(100, 100, 100)
    property var anArray: [1, 2, 3, "four", "five", (function() { return "six"; })]
    property var anObject: { "foo": 10, "bar": 20 }
    property var aFunction: (function() { return "one"; })

    property var aFont: Qt.font({family: "Consolas", pointSize: 30, bold: true})
    property date mDate: "2018-07-19"

    Rectangle
    {
        width: 200 + mInt
        height: 100
        anchors.centerIn: parent
        color: "green"
        Text{
            id: mTextId
            anchors.centerIn: parent
            text: mString
            font.bold: isFemale ? true : false
        }
    }
    /* Run this whenever window is fully loaded */
    Component.onCompleted: {
        print("The value of mString is: " + mString)
        console.log("The value of mInt is: " + mInt)
        if (isFemale)
        {
            console.log("This is female")
        }
        else
            console.log("This is male")
        console.log("The value of mDouble is " + mDouble)
        console.log("The value of mUrl is " + mUrl)
        if (mString == mUrl)
        {
            console.log("They are the same")
        }
        else
            console.log("They are different")
        console.log("Play with var data type -------------------------------------------")
        console.log("value of aNumber is :" +  aNumber)
        console.log("The value of aBool is " + aBool)
        console.log("The components of aRect are: " + aRect.x + aRect.y + aRect.width + aRect.height)
        console.log("Array length : " + anArray.length)
        anArray.forEach(function(value, index)
        {
            if (index == 5)
            {
                /* if variable is a function */
                console.log(value())
            }
            else
                console.log(value)
        })
    }
}
