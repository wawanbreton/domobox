import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Shapes 1.15
import QtQuick.Controls.Material 2.12
import Domobox 1.0

Rectangle {
    id: holiday
    color: domoStyle.componentBackground

    property alias buttonsTop: buttonBack.top

    signal apply()
    signal exit()

    StyledButton {
        id: buttonBack
        anchors.left: parent.left
        anchors.leftMargin: domoStyle.margin
        anchors.bottom: parent.bottom
        anchors.bottomMargin: domoStyle.margin
        icon.source: "/assets/back.svg"
        height: 72
        padding: 11

        onClicked: exit()
    }

    StyledButton {
        anchors.right: parent.right
        anchors.rightMargin: domoStyle.margin
        anchors.bottom: parent.bottom
        anchors.bottomMargin: domoStyle.margin
        icon.source: "/assets/check.svg"
        height: buttonBack.height
        padding: 11

        onClicked: { apply(); exit(); }
    }
}
