import QtQuick 2.0
import QtQml 2.10

Rectangle {
    id: domoComponent
    color: domoStyle.componentBackground
    radius: 24
    state: "collapsed"

    property RoundCorner roundCorner: RoundCorner {}

    signal exit()

    Rectangle {
        id: rectangleTopLeft
        anchors.left: parent.left
        anchors.right: parent.horizontalCenter
        anchors.bottom: parent.verticalCenter
        anchors.top: parent.top
        color: parent.color
        visible: !roundCorner.topLeft
    }

    Rectangle {
        id: rectangleBottomLeft
        anchors.left: parent.left
        anchors.right: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.top: parent.verticalCenter
        color: parent.color
        visible: !roundCorner.bottomLeft
    }

    Rectangle {
        id: rectangleTopRight
        anchors.left: parent.horizontalCenter
        anchors.right: parent.right
        anchors.bottom: parent.verticalCenter
        anchors.top: parent.top
        color: parent.color
        visible: !roundCorner.topRight
    }

    Rectangle {
        id: rectangleBottomRight
        anchors.left: parent.horizontalCenter
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.top: parent.verticalCenter
        color: parent.color
        visible: !roundCorner.bottomRight
    }

    MouseArea {
        // Add a useless mouse area, just to catch mouse event from the MainItem's mask
        anchors.fill: parent
        enabled: domoComponent.state == "expanded"
    }

    transitions: [
         Transition {
            to: "*"

             PropertyAnimation {
                 duration: domoComponent.parent.expandAnimationDuration
                 easing.type: domoComponent.parent.expandAnimationType
                 properties: "opacity"
             }

         }
     ]
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
