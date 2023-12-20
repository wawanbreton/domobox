import QtQuick 2.0

Item {
    property alias selected: imageShutter.enabled

    width: 61
    height: width

    ColoredImage {
        id: imageShutter
        source: "assets/shutter.svg"
        anchors.fill: parent
    }

    MouseArea {
        anchors.fill: parent
        onClicked: imageShutter.enabled = !imageShutter.enabled
    }
}
