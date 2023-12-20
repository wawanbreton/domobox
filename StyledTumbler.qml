import QtQuick 2.0
import QtQuick.Controls 2.15

Tumbler {
    Rectangle {
        x: 6
        y: parent.height * Math.floor(parent.visibleItemCount / 2.0) / parent.visibleItemCount
        width: parent.width - x * 2
        height: 1
        color: domoStyle.foreground
    }

    Rectangle {
        x: 6
        y: parent.height * Math.ceil(parent.visibleItemCount / 2.0) / parent.visibleItemCount
        width: parent.width - x * 2
        height: 1
        color: domoStyle.foreground
    }
}
