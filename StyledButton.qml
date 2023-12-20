import QtQuick 2.0
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0

Button {
    id: button
    width: height
    padding: 0

    background: Rectangle {
        id: rectangle
        color: enabled ? domoStyle.buttonBackground : domoStyle.buttonBackgroundDisabled
        radius: parent.width / 2
        anchors.fill: parent
    }

    DropShadow {
         id: shadowMain
         anchors.fill: rectangle
         radius: 12.0
         samples: 10
         color: rectangle.color
         source: rectangle
         visible: button.enabled && !button.down && !button.checked
     }

    contentItem: Image {
        id: image
        source: parent.icon.source
        anchors.fill: parent
        anchors.leftMargin: button.leftPadding
        anchors.topMargin: button.topPadding
        anchors.rightMargin: button.rightPadding
        anchors.bottomMargin: button.bottomPadding
        visible: false
        fillMode: Image.PreserveAspectFit
    }

    ColorOverlay {
        id: imageOverlay
        anchors.fill: image
        source: image
        color: enabled ? domoStyle.buttonIcon : domoStyle.buttonIconDisabled
    }

    DropShadow {
         anchors.fill: imageOverlay
         radius: 12.0
         samples: 12
         color: imageOverlay.color
         source: imageOverlay
         visible: button.down || button.checked
    }
}


