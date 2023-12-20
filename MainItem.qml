import QtQuick 2.10
import QtGraphicalEffects 1.15

Item {
    id: main

    readonly property int componentsMargin: 31
    property alias expandAnimationDuration: animationExpand.duration
    property alias expandAnimationType: animationExpand.easing.type

    RadialGradient {
        anchors.fill: parent
        gradient: Gradient {
            GradientStop {
                position: 0.0;
                color: domoStyle.backgroundCenter
            }
            GradientStop {
                position: 0.5;
                color: domoStyle.backgroundOuter
            }
        }
    }

    Rectangle {
        id: mask
        anchors.fill: parent
        color: "#000000"
        opacity: 0.0
        visible: opacity > 0.0

        MouseArea {
            anchors.fill: parent
            enabled: main.state != ""
            onClicked: main.state = "";
        }
    }

    TimeComponent {
        id: audioComponent
        width: 462
        anchors.horizontalCenter: main.horizontalCenter
        anchors.bottom: main.bottom
        anchors.top: main.top
        anchors.topMargin: 169
        roundCorner.bottomLeft: false
        roundCorner.bottomRight: false
    }

    AudioComponent {
        id: timeComponent
        anchors.left: audioComponent.left
        anchors.right: audioComponent.right
        anchors.top: main.top
        anchors.bottom: audioComponent.top
        anchors.rightMargin: 0
        anchors.leftMargin: 0
        anchors.bottomMargin: componentsMargin
        roundCorner.topLeft: false
        roundCorner.topRight: false
    }

    TemperatureComponent {
        id: temperatureComponent
        anchors.left: main.left
        anchors.right: audioComponent.left
        anchors.rightMargin: componentsMargin
        anchors.top: main.top
        anchors.bottom: timeComponent.bottom
        roundCorner.topLeft: false
        roundCorner.topRight: false
        roundCorner.bottomLeft: false

        MouseArea {
            anchors.fill: parent;
            enabled: main.state != "temperatureDisplayed"
            onClicked: main.state = "temperatureDisplayed"
            propagateComposedEvents: true
            z: 0.5
        }

        onExit: main.state = ""
    }

    ShutterComponent {
        id: shutterComponent
        anchors.left: audioComponent.right
        anchors.leftMargin: componentsMargin
        anchors.right: main.right
        anchors.top: main.top
        anchors.bottom: timeComponent.bottom
        roundCorner.topLeft: false
        roundCorner.topRight: false
        roundCorner.bottomRight: false

        MouseArea {
            anchors.fill: parent;
            enabled: main.state != "shutterDisplayed"
            onClicked: main.state = "shutterDisplayed"
        }

        onExit: main.state = ""
    }

    WeatherComponent {
        id: weatherComponent
        anchors.left: main.left
        anchors.right: temperatureComponent.right
        anchors.top: audioComponent.top
        anchors.bottom: main.bottom
        roundCorner.topLeft: false
        roundCorner.bottomLeft: false
        roundCorner.bottomRight: false

        MouseArea {
            anchors.fill: parent;
            enabled: main.state != "weatherDisplayed"
            onClicked: main.state = "weatherDisplayed"
        }
    }

    FireComponent {
        id: fireComponent
        anchors.left: audioComponent.right
        anchors.leftMargin: componentsMargin
        anchors.right: main.right
        anchors.bottom: main.bottom
        anchors.top: audioComponent.top
        roundCorner.topRight: false
        roundCorner.bottomRight: false
        roundCorner.bottomLeft: false

        MouseArea {
            anchors.fill: parent;
            enabled: main.state != "fireDisplayed"
            onClicked: main.state = "fireDisplayed"
        }

        Loader {
            id: loaderHoliday
            source: "/Holiday.qml"
            parent: main
            anchors.fill: parent
            z: 3.0
            active: false
        }

        Connections {
             target: loaderHoliday.item

             function onExit() {
                 loaderHoliday.active = false
             }
        }

        Loader {
            id: loaderConfig
            source: "/TemperaturePlanning.qml"
            parent: main
            anchors.fill: parent
            z: 3.0
            active: false
        }

        Connections {
             target: loaderConfig.item

             function onExit() {
                 loaderConfig.active = false
             }
        }

        onExit: main.state = ""
        onShowHoliday: loaderHoliday.active = true
        onShowConfig: loaderConfig.active = true
    }

    states: [
        State {
            name: "weatherDisplayed"

            PropertyChanges {
                target: weatherComponent
                state: "expanded"
                z: 2.0
            }

            AnchorChanges {
                target: weatherComponent
                anchors.right: main.right
            }

            PropertyChanges {
                target: mask
                z: 1.0
                opacity: 0.7
            }
        },

        State {
            name: "temperatureDisplayed"

            PropertyChanges {
                target: temperatureComponent
                state: "expanded"
                anchors.rightMargin: 0
                z: 2.0
            }

            AnchorChanges {
                target: temperatureComponent
                anchors.bottom: main.bottom
                anchors.right: main.right
            }

            AnchorChanges {
                target: weatherComponent
                anchors.right: undefined
            }

            PropertyChanges {
                target: mask
                z: 1.0
                opacity: 0.7
            }
        },

        State {
            name: "fireDisplayed"

            PropertyChanges {
                target: fireComponent
                state: "expanded"
                z: 2.0
                height: 250
                anchors.leftMargin: 0
            }

            AnchorChanges {
                target: fireComponent
                anchors.left: main.left
            }

            PropertyChanges {
                target: mask
                z: 1.0
                opacity: 0.7
            }
        },

        State {
            name: "shutterDisplayed"

            PropertyChanges {
                target: shutterComponent
                state: "expanded"
                z: 2.0
                anchors.leftMargin: 0
            }

            AnchorChanges {
                target: shutterComponent
                anchors.left: main.left
                anchors.bottom: main.bottom
            }
        }
    ]

    transitions: [
         Transition {
            to: "*"

             AnchorAnimation {
                 id: animationExpand
                 duration: 200
                 easing.type: Easing.OutQuad
             }

             PropertyAnimation {
                 id: animationOpacity
                 duration: animationExpand.duration
                 easing: animationExpand.easing
                 properties: "opacity,z,width,anchors.leftMargin,height,anchors.topMargin"
             }
         }
     ]

    Connections {
        target: resource

        function onBacklightOff() {
            main.state = ""
            loaderConfig.active = false
            loaderHoliday.active = false
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;formeditorZoom:0.75;height:480;width:800}
}
##^##*/
