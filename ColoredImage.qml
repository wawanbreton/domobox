import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    implicitWidth: image.implicitWidth
    implicitHeight: image.implicitHeight
    opacity: enabled ? 1.0 : 0.3

    property alias source: image.source
    property alias color: overlay.color

    Image {
        id: image
        source: parent.icon.source
        anchors.fill: parent
        visible: false
        fillMode: Image.PreserveAspectFit
    }

    ColorOverlay {
        id: overlay
        anchors.fill: image
        source: image
        antialiasing: true
        color: domoStyle.foreground
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
