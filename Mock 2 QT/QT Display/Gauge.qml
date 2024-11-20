import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.4
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.3
import QtQuick.Extras.Private 1.0
import QtGraphicalEffects 1.0

CircularGauge {
    id: gauge
    width: 450
    height: width
    property real outerRadius: 450
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
    Behavior on value { NumberAnimation { duration: 50 }}
    style: CircularGaugeStyle {
        labelStepSize: 10
        labelInset: outerRadius / 4
        tickmarkInset: outerRadius / 20
        minorTickmarkInset: outerRadius / 20
        minimumValueAngle: -150
        maximumValueAngle: 150

        background: Rectangle {
            height: gauge.height + 200
            width: gauge.width + 200
            color: "#1E1E1E"
            anchors.centerIn: parent
            radius: 360
            opacity: 0.5

            Canvas {
                property int value: gauge.value

                anchors.fill: parent
                onValueChanged: requestPaint()

                function degreesToRadians(degrees) {
                  return (degrees + 1) * (Math.PI / 180)
                }

                onPaint: {
                    var ctx = getContext("2d");
                    ctx.reset();

                    // Define the gradient colors for the filled arc
                    var gradientColors = [
                        "#32D74B",     // Start color
                        "yellow",  // Middle color (you can add more colors for more segments)
                        "red"    // End color
                    ];

                    // Calculate the start and end angles for the filled arc
                    var startAngle = valueToAngle(gauge.minimumValue) - 90;
                    var endAngle = valueToAngle(gauge.value) - 90;

                    // Loop through the gradient colors and fill the arc segment with each color
                    for (var i = 0; i < gradientColors.length; i++) {
                        var gradientColor = speedColorProvider(gauge.value)
                        speedColor = gradientColor
                        var angle = startAngle + (endAngle - startAngle) * (i / gradientColors.length);

                        ctx.beginPath();
                        ctx.lineWidth = outerRadius * 0.15;
                        ctx.strokeStyle = gradientColor;
                        ctx.arc(outerRadius,
                                outerRadius,
                                outerRadius - ctx.lineWidth / 2 + 25,
                                degreesToRadians(angle),
                                degreesToRadians(endAngle));
                        ctx.stroke();
                    }
                }

            }
        }
        /* Display needle */
        needle: Item {
            visible: true
            y: -outerRadius * 0.95
            height: outerRadius * 0.25
            Image {
                id: needle
                source: "qrc:/assets/needle.svg"
                height: parent.height
                width: height * 0.1
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
            font.pixelSize: Math.max(6, outerRadius * 0.07)
            font.bold: true
            text: styleData.value
            color: styleData.value <= gauge.value ? "yellow" : "darkGray"
            antialiasing: true
        }
        /* Big tick */
        tickmark: Image {
            source: "qrc:/assets/tickmark.svg"
            width: outerRadius * 0.018
            height: outerRadius * 0.15
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
