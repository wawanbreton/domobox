import QtQuick 2.10
import QtQuick.Window 2.10
import QtQuick.Controls 2.1

Window {
    id: window
    width: 800
    height: 480
    visible: true

    MainItem {
        id: mainItem
        anchors.fill: parent
    }

    MouseArea {
        anchors.fill: parent
        enabled: false
        cursorShape: ENV_SIMULATOR ? Qt.ArrowCursor : Qt.BlankCursor
    }
}
