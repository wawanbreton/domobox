import QtQuick 2.14
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import Domobox 1.0

DomoComponent {
    id: domoComponent
    clip: true

    signal showHoliday()
    signal showConfig()

    Image {
        id: imageFire
        anchors.right: parent.right
        anchors.rightMargin: 16
        anchors.top: parent.top
        anchors.topMargin: 32
        height: 140
        source: resource.fire.fireOn ? "/assets/fire.svg" : "/assets/fire-off.svg"
        fillMode: Image.PreserveAspectFit
    }

    ColoredImage {
        id: imageMode
        anchors.horizontalCenter: imageFire.horizontalCenter
        anchors.top: imageFire.bottom
        anchors.topMargin: 24
        width: 72
        source: {
            switch(resource.fire.mode)
            {
                case FireMode.Automatic: return "/assets/automatic.svg";
                case FireMode.Manual:    return "/assets/switch.svg";
                case FireMode.ShortHoliday:
                case FireMode.LongHoliday:
                    return "/assets/suitcase.svg"
            }
        }
    }

    Row {
        anchors.right: parent.right
        anchors.rightMargin: 180
        anchors.top: parent.top
        anchors.topMargin: 80
        spacing: 58

        ExplainedButton {
            iconSource: resource.fire.fireOn ? "/assets/fire-turn-off.svg" : "/assets/match.svg"
            text: resource.fire.fireOn ? "Éteindre\nle feu" : "Allumer\nle feu"

            onClicked: {
                resource.fire.fireOn = !resource.fire.fireOn
                domoComponent.exit()
            }
        }

        ExplainedButton {
            iconSource: resource.fire.modeAuto ? "/assets/switch.svg" : "/assets/automatic.svg"
            text: resource.fire.modeAuto ? "Mode\nmanuel" : "Mode\nautomatique"
            padding: resource.fire.modeAuto ? 20 : 11

            onClicked: {
                resource.fire.modeAuto = !resource.fire.modeAuto
                domoComponent.exit()
            }
        }

        ExplainedButton {
            iconSource: "/assets/suitcase.svg"
            text: qsTr("Départ\nen vacances")
            padding: 8
            bottomPadding: 18

            onClicked: {
                domoComponent.showHoliday()
                domoComponent.exit()
            }
        }

        ExplainedButton {
            iconSource: "/assets/settings.svg"
            text: qsTr("Configuration")

            onClicked: {
                domoComponent.showConfig()
                domoComponent.exit()
            }
        }
    }

    states: [
        State { name: "collapsed" },
        State { name: "expanded" }
    ]
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
