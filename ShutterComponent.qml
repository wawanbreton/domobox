import QtQuick 2.0
import Domobox 1.0

DomoComponent {
    id: domoComponent
    state: "collapsed"
    clip: true

    Column
    {
        id: column
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.topMargin: 17
        width: 116
        spacing: 18

        ColoredImage {
            id: imageShutter
            height: 98
            width: height
            source: "assets/shutter.svg"

            MouseArea {
                anchors.fill: parent
                enabled: domoComponent.state == "expanded"
                onClicked: domoComponent.exit()
            }
        }

        StyledButton {
            id: buttonUp
            width: imageShutter.width
            height: width
            anchors.horizontalCenter: imageShutter.horizontalCenter
            icon.source: "/assets/up.svg"
            visible: domoComponent.state == "expanded"

            onClicked: { sendCommand(ShutterCommand.Up); domoComponent.exit(); }
        }

        StyledButton {
            id: buttonStop
            width: imageShutter.width
            height: width
            anchors.horizontalCenter: imageShutter.horizontalCenter
            icon.source: "/assets/stop.svg"
            visible: domoComponent.state == "expanded"

            onClicked: { sendCommand(ShutterCommand.Stop); domoComponent.exit(); }
        }

        StyledButton {
            id: buttonDown
            width: imageShutter.width
            height: width
            anchors.horizontalCenter: imageShutter.horizontalCenter
            icon.source: "/assets/down.svg"
            visible: domoComponent.state == "expanded"

            onClicked: { sendCommand(ShutterCommand.Down); domoComponent.exit(); }
        }
    }

    ColoredImage {
        id: imageMap
        anchors.top: parent.top
        anchors.topMargin: column.anchors.topMargin
        anchors.right: column.left
        anchors.rightMargin: 26
        source: "assets/map.svg"
        width: 636
        height: 446

        StyledButton {
            id: buttonAll
            width: buttonUp.width
            height: width
            x: 288
            y: 160

            icon.source: "/assets/all.svg"

            onClicked: {
                var selected = !shutterKitchen.selected
                shutterChild.selected = selected
                shutterKitchen.selected = selected
                shutterParents.selected = selected
                shutterStairs.selected = selected
                shutterLivingRoomLeft.selected = selected
                shutterLivingRoomRight.selected = selected
            }
        }

        ShutterSelector {
            id: shutterChild
            rotation: -90
            x: 562
            y: 58
        }

        ShutterSelector {
            id: shutterKitchen
            rotation: 180
            x: 180
            y: 12
        }

        ShutterSelector {
            id: shutterParents
            x: 486
            y: 373
        }

        ShutterSelector {
            id: shutterStairs
            x: 320
            anchors.bottom: shutterParents.bottom
        }

        ShutterSelector {
            id: shutterLivingRoomLeft
            width: 77
            x: 180
            anchors.bottom: shutterParents.bottom
        }

        ShutterSelector {
            id: shutterLivingRoomRight
            width: shutterLivingRoomLeft.width
            anchors.right: shutterLivingRoomLeft.left
            anchors.rightMargin: 20
            anchors.bottom: shutterLivingRoomLeft.bottom
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

    function sendCommand(command) {
        var shutters = []

        if(shutterStairs.selected)
        {
            shutters.push(Shutter.Stairs)
        }
        if(shutterLivingRoomLeft.selected)
        {
            shutters.push(Shutter.LivingRoomLeft)
        }
        if(shutterLivingRoomRight.selected)
        {
            shutters.push(Shutter.LivingRoomRight)
        }
        if(shutterKitchen.selected)
        {
            shutters.push(Shutter.Kitchen)
        }
        if(shutterChild.selected)
        {
            shutters.push(Shutter.RoomChild)
        }
        if(shutterParents.selected)
        {
            shutters.push(Shutter.RoomParent)
        }

        resource.shutter.sendShuttersCommand(command, shutters)
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
