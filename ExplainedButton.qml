import QtQuick 2.0

Item {
    width: button.width
    height: textExplanation.y + textExplanation.height

    property alias iconSource: button.icon.source
    property alias text: textExplanation.text
    property alias padding: button.padding
    property alias topPadding: button.topPadding
    property alias bottomPadding: button.bottomPadding
    property alias leftPadding: button.leftPadding
    property alias rightPadding: button.rightPadding

    signal clicked()

    StyledButton {
        id: button
        height: 100

        onClicked: parent.clicked()
    }

    Text {
        id: textExplanation
        anchors.top: button.bottom
        anchors.topMargin: 16
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: domoStyle.fontSizeSmall
        horizontalAlignment: Text.AlignHCenter
        color: domoStyle.foreground
    }
}
