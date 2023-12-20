import QtQuick 2.0
import Domobox 1.0

DomoComponent {
    id: domoComponent
    clip: true

    NumberAnimation {
        id: animationTurnOn
        from: -0.49
        to: 3.49
        running: false
        target: imageState
        property: "index"
        loops: Animation.Infinite
        duration: 1050
    }

    NumberAnimation {
        id: animationTurnOff
        from: animationTurnOn.to
        to: animationTurnOn.from
        running: false
        target: animationTurnOn.target
        property: animationTurnOn.property
        loops: animationTurnOn.loops
        duration: animationTurnOn.duration
    }

    Row {
        anchors.right: parent.right
        anchors.rightMargin: 15
        spacing: 80

        Column {
            anchors.bottom: parent.bottom
            anchors.top: parent.top
            anchors.topMargin: 14

            ColoredImage {
                source: "/assets/clock.svg"
                height: 80
                width: 80
            }

            Text {
                text: resource.wifi.endTimeStr
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: domoStyle.fontSizeMedium
                color: domoStyle.foreground
            }
        }

        Column {
            anchors.bottom: parent.bottom
            anchors.top: parent.top
            anchors.topMargin: 14

            ColoredImage {
                source: "/assets/stopwatch.svg"
                height: 80
                width: 80
            }

            Text {
                text: resource.wifi.remainingTimeStr
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: domoStyle.fontSizeMedium
                color: domoStyle.foreground
            }
        }

        StyledButton {
            id: buttonAdd
            height: buttonDisable.width
            anchors.verticalCenter: parent.verticalCenter
            icon.source: "/assets/add.svg"

            onClicked: popupAddTime.open()
        }

        StyledButton {
            id: buttonDisable
            height: imageState.width - 22
            anchors.verticalCenter: parent.verticalCenter
            icon.source: "/assets/wifi-turn-off.svg"
            enabled: resource.wifi.state == WifiState.Up

            onClicked: { resource.wifi.deactivate(); domoComponent.exit(); }
        }

        ColoredImage {
            id: imageState

            property int index: 0

            width: 105
            anchors.verticalCenter: parent.verticalCenter
            anchors.bottomMargin: 2
            source: "assets/wifi-%1.svg".arg(index)
        }
    }

    PopupAddWifiTime {
        id: popupAddTime
        parent: domoComponent.parent

        //onClosed: domoComponent.exit()
    }

    Connections {
        target: resource.wifi

        function onStateChanged() {
            updateState()
        }
    }

    states: [
        State {
            name: "collapsed"
        },

        State {
            name: "expanded"
        }
    ]

    onStateChanged: {
        if(state == "collapsed")
        {
            popupAddTime.close();
        }
    }

    Component.onCompleted: updateState()

    function updateState() {
        var state = resource.wifi.state
        imageState.index = (state === WifiState.Up) ? 3 : 0
        animationTurnOn.running = (state === WifiState.TurningUp)
        animationTurnOff.running = (state === WifiState.TurningDown)
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
