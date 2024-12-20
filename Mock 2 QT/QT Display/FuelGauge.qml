import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.4
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.3
import QtQuick.Extras.Private 1.0
import QtGraphicalEffects 1.0

CircularGauge {
    id: fuelGauge
    property string speedColor: "#32D74B"
    function fuelLevelProvider(value){
        var redIdx = 255;
        var greenIdx = 255;
        if(value < 50 ){
            greenIdx = value / 50
            redIdx = 1
        } else
        if(value > 50 ){
            greenIdx = 1
            redIdx =(100 - value) / 50
        } else
        {
            redIdx = 1
            greenIdx= 1
        }
        return Qt.rgba(redIdx, greenIdx, 0, 1);
    }

    Behavior on value { NumberAnimation { duration: 50 }}
    style: CircularGaugeStyle {
        labelStepSize: 10
        labelInset: outerRadius / 2.2
        tickmarkInset: outerRadius / 4.2
        minorTickmarkInset: outerRadius / 4.2
        minimumValueAngle: -60
        maximumValueAngle: 60

        background: Rectangle {
            implicitHeight: fuelGauge.height
            implicitWidth: fuelGauge.width
            color: "#1E1E1E"
            anchors.centerIn: parent
            radius: 360
            opacity: 1
            Canvas {
                anchors.fill: parent
                property int value: fuelGauge.value


                onValueChanged:requestPaint()
                function degreesToRadians(degrees) {
                  return degrees * (Math.PI / 180);
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
                    var startAngle = valueToAngle(fuelGauge.minimumValue) - 90;
                    var endAngle = valueToAngle(fuelGauge.value) - 90;

                    // Loop through the gradient colors and fill the arc segment with each color
                    for (var i = 0; i < gradientColors.length; i++) {
                        var gradientColor = fuelLevelProvider(fuelGauge.value)
                        speedColor = gradientColor
                        var angle = startAngle + (endAngle - startAngle) * (i / gradientColors.length);

                        ctx.beginPath();
                        ctx.lineWidth = outerRadius * 0.225;
                        ctx.strokeStyle = gradientColor;
                        ctx.arc(outerRadius,
                                outerRadius,
                                outerRadius - ctx.lineWidth / 2,
                                degreesToRadians(angle),
                                degreesToRadians(endAngle));
                        ctx.stroke();
                    }
                }
            }
        }

        needle: Item {
            visible: true
            y: -outerRadius * 0.78
            height: outerRadius * 0.27
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
        /* Fuel image */
        foreground: Item {
            id: foreground
            RowLayout{
                anchors.centerIn: parent
                Image {
                    id: fuelSymbol
                    x: parent.implicitHeight / 5 + 100
                    y: parent.implicitWidth / 5 + 100
                    width: 72
                    height: 72
                    source: "qrc:/assets/fuel.svg"
                    opacity: 1
                    property alias fuelOpacity: fuelSymbol.opacity

                }
                Label{
                    text: fuelGauge.value.toFixed(0) + '%'
                    font.pixelSize: 85
                    font.family: "Inter"
                    color: fuelLevelProvider(fuelGauge.value)
                    font.bold: Font.DemiBold
                    Layout.alignment: Qt.AlignHCenter
                }
                Timer {
                    id: blinkTimer
                        interval: 750
                        running: true
                        repeat: true
                        onTriggered:{
                            if (fuelGauge.value <= 10)
                                fuelSymbol.opacity = fuelSymbol.opacity === 1 ? 0 : 1;
                            else
                                fuelSymbol.opacity = 1 
                        }
                    }
            }
        }


        tickmarkLabel:  Text {
            font.pixelSize: Math.max(6, outerRadius * 0.05)
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
