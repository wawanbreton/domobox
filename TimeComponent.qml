import QtQuick 2.0

DomoComponent {
    Text {
        id: textTime
        anchors.top: parent.top
        text: resource.time.time
        font.pixelSize: domoStyle.fontSizeHuge
        anchors.topMargin: 32
        anchors.horizontalCenter: parent.horizontalCenter
        color: domoStyle.foreground
    }

    Text {
        id: textDate
        anchors.top: textTime.bottom
        anchors.topMargin: 32
        anchors.horizontalCenter: parent.horizontalCenter
        text: resource.time.date
        font.pixelSize: domoStyle.fontSizeBig
        color: domoStyle.foreground
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
