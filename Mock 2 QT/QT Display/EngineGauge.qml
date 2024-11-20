import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.4
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.3
import QtQuick.Extras.Private 1.0
import QtGraphicalEffects 1.0

CircularGauge {
    id: fuelGauge
    Behavior on value { NumberAnimation { duration: 500 }}
    style: CircularGaugeStyle {
        labelStepSize: 2
        labelInset: outerRadius / 2.2
        tickmarkInset: outerRadius / 4.2
        minorTickmarkInset: outerRadius / 4.2
        minimumValueAngle: -140
        maximumValueAngle: -30

        background: Rectangle {
            implicitHeight: fuelGauge.height
            implicitWidth: fuelGauge.width
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
            color: styleData.value <= fuelGauge.value ? "white" : "#777776"
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
            implicitWidth: outerRadius * 0.01
            implicitHeight: outerRadius * 0.03

            antialiasing: true
            smooth: true
            color: styleData.value <= fuelGauge.value ? "white" : "darkGray"
        }
    }
}
