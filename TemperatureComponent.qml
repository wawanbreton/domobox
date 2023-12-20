import QtQuick 2.0
import Domobox 1.0

DomoComponent {
    id: domoComponent
    state: "collapsed"
    clip: true

    Item {
        id: itemCollapsed
        anchors.fill: parent
        visible: opacity > 0.0

        ColoredImage {
            id: imageHouse
            y: 12
            height: 64
            source: "assets/house.svg"
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Text {
            id: textTemperature
            text: resource.temperature.temperatureStr
            font.pixelSize: domoStyle.fontSizeBig
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: imageHouse.bottom
            anchors.topMargin: 6
            color: domoStyle.foreground
        }
    }

    TemperatureChart {
        id: itemExpanded
        width: domoComponent.parent.width
        height: domoComponent.parent.height
        visible: opacity > 0.0
    }

    MouseArea {
        anchors.fill: parent
        enabled: domoComponent.state == "expanded"
        onClicked: domoComponent.exit()
    }

    states: [
        State {
            name: "collapsed"

            PropertyChanges {
                target: itemCollapsed
                opacity: 1.0
            }

            PropertyChanges {
                target: itemExpanded
                opacity: 0.0
            }
        },

        State {
            name: "expanded"

            PropertyChanges {
                target: itemCollapsed
                opacity: 0.0
            }

            PropertyChanges {
                target: itemExpanded
                opacity: 1.0
            }
        }
    ]

}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
