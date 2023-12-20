import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.0

SpinBox {
    id: spinBoxStart

    from: 0
    to: 48
    Material.accent: domoStyle.foreground
    Material.foreground: domoStyle.foreground

    textFromValue: function(value, locale) {
        return getDate().toLocaleTimeString("HH:mm")
    }

    function getValue(time) {
        return time.getHours() * 2 + time.getMinutes() / 30
    }

    function getDate() {
        var date = new Date(0)
        date.setHours(value / 2)
        date.setMinutes((value % 2) * 30)
        return date
    }

    Rectangle {
        anchors.fill: parent
        color: "transparent"
        border.width: 1.0
        border.color: domoStyle.foreground
    }
}
