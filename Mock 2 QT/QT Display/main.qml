import QtQuick 2.12
import CustomControls 1.0
import QtQuick.Window 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import "./"

ApplicationWindow {
    width: 1600
    height: 1080
    visible: true
    title: qsTr("Car DashBoard")
    color: "#1E1E1E"
    visibility: "FullScreen"
    property int turn: 1
    property bool overSpeed: false
    /* Display speed color */
    function speedColor(value){
        if(value < 60 ){
            return "green"
        } else if(value > 60 && value < 150){
            return "yellow"
        }else{
            return "Red"
        }
    }
    /* Display time */
    Timer {
        interval: 500
        running: true
        repeat: true
        onTriggered:{
            currentTime.text = Qt.formatDateTime(new Date(), "hh:mm:ss")
        }
    }
    /* Ctrl Q to qujit */
    Shortcut {
        sequence: "Ctrl+Q"
        context: Qt.ApplicationShortcut
        onActivated: Qt.quit()
    }
    /* Display surounding oval */
    Image {
        id: dashboard
        width: parent.width
        height: parent.height
        anchors.centerIn: parent
        source: "qrc:/assets/Dashboard.svg"

        /* Top Bar Of Screen */
        Image {
            id: topBar
            width: 1357
            source: "qrc:/assets/Vector 70.svg"
            anchors{
                top: parent.top
                topMargin: 26.50
                horizontalCenter: parent.horizontalCenter
            }
            /* Display front light */
            Image {
                id: headLight
                property bool indicator: false
                width: 42.5
                height: 38.25
                anchors{
                    top: parent.top
                    topMargin: 25
                    leftMargin: 230
                    left: parent.left
                }
                source: indicator ? "qrc:/assets/Low beam headlights.svg" : "qrc:/assets/Low_beam_headlights_white.svg"
                Behavior on indicator { NumberAnimation { duration: 300 }}
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        headLight.indicator = !headLight.indicator
                    }
                }
            }
            /* Display current time */
            Label
            {
                id: currentTime
                text: Qt.formatDateTime(new Date(), "hh:mm:ss")
                font.pixelSize: 32
                font.family: "Inter"
                font.bold: Font.DemiBold
                color: "#FFFFFF"
                anchors.top: parent.top
                anchors.topMargin: 25
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Label{
                id: currentDate
                text: Qt.formatDateTime(new Date(), "dd/MM/yyyy")
                font.pixelSize: 32
                font.family: "Inter"
                font.bold: Font.DemiBold
                color: "#FFFFFF"
                anchors.right: parent.right
                anchors.rightMargin: 230
                anchors.top: parent.top
                anchors.topMargin: 25
            }
        }

        /*  Speed Limit Label */
        Rectangle{
            id: speedLimit
            width: 130
            height: 130
            radius: height/2
            color: "#D9D9D9"
            border.color: speedColor(maxspeedGauge.text)
            border.width: 10
            opacity: 1
            anchors.horizontalCenter: fuelGauge.horizontalCenter
            anchors.bottom: fuelGauge.top
            anchors.bottomMargin: 50

            Label{
                id: maxspeedGauge
                property int maxOverSpeed: getRandomInt(50, speedGauge.maximumValue).toFixed(0)
                text: maxOverSpeed
                font.pixelSize: 45
                font.family: "Inter"
                font.bold: Font.Bold
                color: "#01E6DE"
                anchors.centerIn: parent
                
                function getRandomInt(min, max) {
                    return Math.floor(Math.random() * (max - min + 1)) + min;
                }
            }
        }

        Image {
            anchors{
                bottom: car.top
                bottomMargin: 30
                horizontalCenter:car.horizontalCenter
            }
            source: "qrc:/assets/Model 3.png"
        }

        Image {
            id: car
            anchors{
                bottom: parent.bottom
                bottomMargin: 30
                horizontalCenter:parent.horizontalCenter
            }
            source: "qrc:/assets/Car.svg"
        }

        /* Left Road */
        Image
        {
            id: leftRoad
            width: 127
            height: 397
            anchors
            {
                left: car.left
                leftMargin: 100
                bottom: parent.bottom
                bottomMargin: 26.50
            }
            source: "qrc:/assets/Vector 2.svg"
        }

        Item{
            anchors{
                left: topBar.left
                leftMargin: 300
                top: topBar.top
                topMargin: 25
            }

            RowLayout{
                spacing: 3
                Label{
                    text: "30.6"
                    font.pixelSize: 32
                    font.family: "Inter"
                    font.bold: Font.Normal
                    font.capitalization: Font.AllUppercase
                    color: "#FFFFFF"
                }

                Label{
                    text: "°C"
                    font.pixelSize: 32
                    font.family: "Inter"
                    font.bold: Font.Normal
                    font.capitalization: Font.AllUppercase
                    opacity: 0.6
                    color: "#FFFFFF"
                }
            }
        }

        /* Right Road */
        Image {
            id: rightRoad
            width: 127
            height: 397
            anchors{
                right: car.right
                rightMargin: 100
                bottom: parent.bottom
                bottomMargin: 26.50
            }
            source: "qrc:/assets/Vector 1.svg"
        }

        /* Right Side gear */
        RowLayout{
            spacing: 20
            anchors{
                right: parent.right
                rightMargin: 350
                bottom: parent.bottom
                bottomMargin: 26.50 + 65
            }

            Label{
                text: "Ready"
                font.pixelSize: 32
                font.family: "Inter"
                font.bold: Font.Normal
                font.capitalization: Font.AllUppercase
                color: "#32D74B"
            }

            Label{
                text: "P"
                font.pixelSize: 32
                font.family: "Inter"
                font.bold: Font.Normal
                font.capitalization: Font.AllUppercase
                color: "#FFFFFF"
            }

            Label{
                text: "R"
                font.pixelSize: 32
                font.family: "Inter"
                font.bold: Font.Normal
                font.capitalization: Font.AllUppercase
                opacity: 0.2
                color: "#FFFFFF"
            }
            Label{
                text: "N"
                font.pixelSize: 32
                font.family: "Inter"
                font.bold: Font.Normal
                font.capitalization: Font.AllUppercase
                opacity: 0.2
                color: "#FFFFFF"
            }
            Label{
                text: "D"
                font.pixelSize: 32
                font.family: "Inter"
                font.bold: Font.Normal
                font.capitalization: Font.AllUppercase
                opacity: 0.2
                color: "#FFFFFF"
            }
        }

        /* Left Side Icons */
        Image
        {
            id:forthLeftIndicator
            property bool parkingLightOn: true
            width: 72
            height: 62
            anchors{
                left: parent.left
                leftMargin: 175
                bottom: thirdLeftIndicator.top
                bottomMargin: 25
            }
            Behavior on parkingLightOn { NumberAnimation { duration: 300 }}
            source: parkingLightOn ? "qrc:/assets/Parking lights.svg" : "qrc:/assets/Parking_lights_white.svg"
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    forthLeftIndicator.parkingLightOn = !forthLeftIndicator.parkingLightOn
                }
            }
        }

        Image {
            id:thirdLeftIndicator
            property bool lightOn: true
            width: 52
            height: 70.2
            anchors{
                left: parent.left
                leftMargin: 145
                bottom: secondLeftIndicator.top
                bottomMargin: 25
            }
            source: lightOn ? "qrc:/assets/Lights.svg" : "qrc:/assets/Light_White.svg"
            Behavior on lightOn { NumberAnimation { duration: 300 }}
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    thirdLeftIndicator.lightOn = !thirdLeftIndicator.lightOn
                }
            }
        }

        Image {
            id:secondLeftIndicator
            property bool headLightOn: true
            width: 51
            height: 51
            anchors{
                left: parent.left
                leftMargin: 125
                bottom: firstLeftIndicator.top
                bottomMargin: 30
            }
            Behavior on headLightOn { NumberAnimation { duration: 300 }}
            source:headLightOn ?  "qrc:/assets/Low beam headlights.svg" : "qrc:/assets/Low_beam_headlights_white.svg"

            MouseArea{
                anchors.fill: parent
                onClicked: {
                    secondLeftIndicator.headLightOn = !secondLeftIndicator.headLightOn
                }
            }
        }

        Image {
            id:firstLeftIndicator
            property bool rareLightOn: false
            width: 51
            height: 51
            anchors{
                left: parent.left
                leftMargin: 100
                verticalCenter: speedGauge.verticalCenter
            }
            source: rareLightOn ? "qrc:/assets/Rare_fog_lights_red.svg" : "qrc:/assets/Rare fog lights.svg"
            Behavior on rareLightOn { NumberAnimation { duration: 300 }}
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    firstLeftIndicator.rareLightOn = !firstLeftIndicator.rareLightOn
                }
            }
        }

        /*Right Side Icons*/

        Image {
            id:forthRightIndicator
            property bool indicator: true
            width: 56.83
            height: 36.17
            anchors{
                right: parent.right
                rightMargin: 195
                bottom: thirdRightIndicator.top
                bottomMargin: 50
            }
            source: indicator ? "qrc:/assets/FourthRightIcon.svg" : "qrc:/assets/FourthRightIcon_red.svg"
            Behavior on indicator { NumberAnimation { duration: 300 }}
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    forthRightIndicator.indicator = !forthRightIndicator.indicator
                }
            }
        }

        Image {
            id:thirdRightIndicator
            property bool indicator: true
            width: 56.83
            height: 36.17
            anchors{
                right: parent.right
                rightMargin: 155
                bottom: secondRightIndicator.top
                bottomMargin: 50
            }
            source: indicator ? "qrc:/assets/thirdRightIcon.svg" : "qrc:/assets/thirdRightIcon_red.svg"
            Behavior on indicator { NumberAnimation { duration: 300 }}
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    thirdRightIndicator.indicator = !thirdRightIndicator.indicator
                }
            }
        }

        Image {
            id:secondRightIndicator
            property bool indicator: true
            width: 56.83
            height: 36.17
            anchors{
                right: parent.right
                rightMargin: 125
                bottom: firstRightIndicator.top
                bottomMargin: 50
            }
            source: indicator ? "qrc:/assets/SecondRightIcon.svg" : "qrc:/assets/SecondRightIcon_red.svg"
            Behavior on indicator { NumberAnimation { duration: 300 }}
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    secondRightIndicator.indicator = !secondRightIndicator.indicator
                }
            }
        }

        Image {
            id:firstRightIndicator
            property bool sheetBelt: true
            width: 36
            height: 45
            anchors{
                right: parent.right
                rightMargin: 100
                verticalCenter: speedGauge.verticalCenter
            }
            source: sheetBelt ? "qrc:/assets/FirstRightIcon.svg" : "qrc:/assets/FirstRightIcon_grey.svg"
            Behavior on sheetBelt { NumberAnimation { duration: 300 }}
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    firstRightIndicator.sheetBelt = !firstRightIndicator.sheetBelt
                }
            }
        }
        /* Engine Gauge */
        EngineGauge {
            id: engineGauge
            anchors{
                verticalCenter: parent.verticalCenter
                left: parent.left
                leftMargin: parent.width / 7
            }
            width: 450
            height: 450
            value: 0
            property int curSpeed: speedGauge.value
            property int preSpeed: speedGauge.preValue
            minimumValue: 0
            maximumValue: 10
            onCurSpeedChanged: {
                /* Speeding */
                if(speedGauge.value - speedGauge.preValue > 0)
                {
                    value = curSpeed / speedGauge.maximumValue * 10 + 3
                    if (value > maximumValue)
                        value = maximumValue
                }
                else
                {
                    value = curSpeed / speedGauge.maximumValue * 10
                }
                speedGauge.preValue = speedGauge.value
            }
            Timer {
                id: blinkTimer
                property int stackTime: 0
                interval: 50
                running: true
                repeat: true
                onTriggered:{
                    if (speedGauge.value === speedGauge.preValue)
                    {
                        stackTime ++;
                        if (stackTime >= 5)
                        {
                            if (speedGauge.value === 0)
                                engineGauge.value = 0.1
                            if (speedGauge.value < 50)
                                engineGauge.value = speedGauge.value / 25 + 0.7
                            else
                                engineGauge.value = 10 - (speedGauge.maximumValue - speedGauge.value - 60)/ 25
                            if (stackTime % 20 === 0)
                                engineGauge.value += 1
                        }
                    }
                    else
                        stackTime = 0
                }
            }
        }

        /* Speedometer */
        Gauge {
            id: speedGauge
            value: 0
            property int preValue: 0
            property int engineStatus: 0
            maximumValue: 250
            anchors.top: parent.top
            anchors.topMargin:Math.floor(parent.height * 0.25) - 100
            anchors.horizontalCenter: parent.horizontalCenter
            focus: true
            onValueChanged:
            {
                if (value > maxspeedGauge.maxOverSpeed)
                    overSpeed = true
                else
                    overSpeed = false
            }
        }
        /* Fuel Gauge */
        FuelGauge {
            id: fuelGauge
            anchors {
                verticalCenter: parent.verticalCenter
                right: parent.right
                rightMargin: parent.width / 10
            }
            width: 450
            height: 450
            value: 20
            minimumValue: 0
            maximumValue: 100
            Component.onCompleted: forceActiveFocus()
            focus: true
        }
        /* Turn right */
        Image {
            id: turnRight
            x: 500
            width: 100
            height: 100
            opacity: 0
            anchors{
                right: fuelGauge.left
                top: speedGauge.top
            }
            source: "qrc:/assets/right-arrow-on.png"
        }
        /* Turn Left */
        Image {
            x: 0
            id: turnLeft
            width: 100
            height: 100
            opacity: 0
            anchors{
                right: engineGauge.right
                top: speedGauge.top
            }
            source: "qrc:/assets/left-arrow-on.png"
        }
        Timer {
            interval: 750
            running: true
            repeat: true
            onTriggered:{
                /* Not turn */
                if (turn === 1)
                {
                    turnLeft.opacity = 0;
                    turnRight.opacity = 0;
                }   /* Turn left */
                else  if (turn === 0)
                {
                    turnLeft.opacity = (turnLeft.opacity === 1) ? 0 : 1;
                    turnRight.opacity = 0;
                }   /* Turn right */
                else if (turn === 2)
                {
                    turnLeft.opacity = 0;
                    turnRight.opacity = (turnRight.opacity === 1) ? 0 : 1
                }
                if (overSpeed === true)
                {
                    speedLimit.opacity = (speedLimit.opacity === 1) ? 0 : 1
                }
                else
                    speedLimit.opacity = 1
            }
        }
        /* Keyboard handler */
        Keys.onPressed: {
            /* Key A and D to decrease/increase fuel */
            if (event.key === Qt.Key_A) {
                fuelGauge.value = Math.min(fuelGauge.value - 1, fuelGauge.value)
            }
            else
            if (event.key === Qt.Key_D) {
                fuelGauge.value = Math.max(0, fuelGauge.value + 1)
            } else
            /* Key W and S to decrease/increase speed */
            if (event.key === Qt.Key_W) {
                speedGauge.value = Math.max(0, speedGauge.value + 1)
            } else
            if (event.key === Qt.Key_S) {
                speedGauge.value = Math.min(speedGauge.value - 1, speedGauge.value)
            } else
            /* Turn left or right */
            if (event.key === Qt.Key_Right) {
                turn ++
                if (turn > 2)
                    turn = 2
            } else
            if (event.key === Qt.Key_Left) {
                turn --
                if (turn < 0)
                    turn = 0
            }
        }
    }
}
