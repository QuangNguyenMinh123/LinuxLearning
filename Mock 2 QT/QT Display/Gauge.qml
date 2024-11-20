import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.4
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.3
import QtQuick.Extras.Private 1.0
import QtGraphicalEffects 1.0

CircularGauge {
    id: gauge
    width: 500
    height: width
    property real outerRadius: width
    property string speedColor: "#32D74B"
    function speedColorProvider(value){
        if(value < 60 ){
            return "#32D74B"
        } else if(value > 60 && value < 150){
            return "yellow"
        }else{
            return "Red"
        }
    }
    Behavior on value { NumberAnimation { duration: 1 }}
    style: CircularGaugeStyle {
        labelStepSize: 10
        labelInset: outerRadius / 3.5
        tickmarkInset: outerRadius / 10
        minorTickmarkInset: outerRadius / 10
        minimumValueAngle: -130
        maximumValueAngle: 130

        background: Rectangle
        {
            height: gauge.height
            width: gauge.width
            color: "#1E1E1E"
            anchors.centerIn: parent
            radius: 360
            opacity: 1
        }
        /* Display speed */
        foreground: Item {
            ColumnLayout{
                anchors.centerIn: parent
                Label{
                    text: gauge.value.toFixed(0)
                    font.pixelSize: 85
                    font.family: "Inter"
                    color: "#01E6DE"
                    font.bold: Font.DemiBold
                    Layout.alignment: Qt.AlignHCenter
                }

                Label{
                    text: "KPH"
                    font.pixelSize: 46
                    font.family: "Inter"
                    color: "#01E6DE"
                    font.bold: Font.Normal
                    Layout.alignment: Qt.AlignHCenter
                }
            }
        }
        /* Display static speed */
        tickmarkLabel:  Text {
            font.pixelSize: Math.max(6, outerRadius * 0.06)
            font.bold: true
            text: styleData.value
            color: styleData.value <= gauge.value ? "yellow" : "darkGray"
            antialiasing: true
        }
        /* Big tick */
        tickmark: Image {
            source: "qrc:/assets/tickmark.svg"
            width: outerRadius * 0.018
            height: outerRadius * 0.1
            antialiasing: true
            asynchronous: true
        }
        /* Small Tick */
        minorTickmark: Rectangle {
            implicitWidth: outerRadius * 0.01
            implicitHeight: outerRadius * 0.03

            antialiasing: true
            smooth: true
            color: styleData.value <= gauge.value ? "white" : "darkGray"
        }
    }
}
