import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.4
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.3
import QtQuick.Extras.Private 1.0
import QtGraphicalEffects 1.0

CircularGauge {
    id: engineGauge
    Behavior on value { NumberAnimation { duration: 500 }}
    function fuelLevelProvider(value){
        var redIdx = 255;
        var greenIdx = 255;
        if(value < 5 ){
            greenIdx = value / 5
            redIdx = 1
        } else
        if(value > 5 ){
            greenIdx = 1
            redIdx =(10 - value) / 5
        } else
        {
            redIdx = 1
            greenIdx= 1
        }
        return Qt.rgba(redIdx, greenIdx, 0, 1);
    }
    style: CircularGaugeStyle {
        labelStepSize: 2
        labelInset: outerRadius / 2.2
        tickmarkInset: outerRadius / 4.2
        minorTickmarkInset: outerRadius / 4.2
        minimumValueAngle: -140
        maximumValueAngle: -30

        background: Rectangle {
            implicitHeight: engineGauge.height
            implicitWidth: engineGauge.width
            color: "#1E1E1E"
            anchors.centerIn: parent
            radius: 360
            opacity: 1
        }

        needle: Item {
            y: -outerRadius * 0.4
            height: outerRadius * 0.06
            Image {
                id: needle
                source: "qrc:/assets/Rectangle 4.svg"
                width: height * 0.06
                asynchronous: true
                antialiasing: true
            }

            Glow {
              anchors.fill: needle
              radius: 5
              samples: 10
              color: "white"
              source: needle
          }
        }

        tickmarkLabel:  Text {
            font.pixelSize: Math.max(6, outerRadius * 0.1)
            text: styleData.value
            color: styleData.value <= engineGauge.value ? "white" : "#777776"
            antialiasing: true
        }

        tickmark: Image {
            source: "qrc:/assets/tickmark.svg"
            width: outerRadius * 0.018
            height: outerRadius * 0.15
            antialiasing: true
            asynchronous: true
        }

        minorTickmark: Rectangle {
            implicitWidth: outerRadius * 0.02
            implicitHeight: outerRadius * 0.03

            antialiasing: true
            smooth: true
            color: styleData.value <= engineGauge.value ? fuelLevelProvider(engineGauge.value) : "darkGray"
        }

    }
}
