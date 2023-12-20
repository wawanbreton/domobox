import QtQuick 2.0
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.0
import Domobox 1.0

DomoComponent {
    id: domoComponent
    state: "collapsed"

    ColoredImage {
        id: imageSpeaker
        source: resource.audio.state == AudioState.Bluetooth ? "/assets/speaker-on.svg" : "/assets/speaker-off.svg"
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 100
        height: buttonBluetooth.height
    }

    StyledButton {
        id: buttonBluetooth
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: imageSpeaker.anchors.leftMargin
        width: 98
        height: width
        icon.source: "assets/bluetooth.svg"
        checkable: true
        onToggled: resource.audio.bluetoothButtonToggled(checked)
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
