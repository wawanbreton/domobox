import QtQuick 2.0
import QtGraphicalEffects 1.15
import QtQuick.Controls 2.15

AbstractButton {
    id: itemHotPeriod

    required property var period

    visible: period.active
    x: timeToPosition(period.begin, false)
    width: timeToPosition(period.end, true) - x
    checkable: true

    Rectangle {
        id: rectangle
        anchors.fill: parent
        color: domoStyle.hot
        radius: 20
    }

    DropShadow {
         anchors.fill: rectangle
         radius: 12.0
         samples: 17
         color: "#222222"
         source: rectangle
         visible: itemHotPeriod.checked
    }

    Connections {
        target: period
        ignoreUnknownSignals: true

        function onActiveChanged() {
            if(!period.active)
            {
                checked = false;
            }
        }
    }

    function timeToPosition(time, isEnd) {
        var factor = 1.0
        if(!isEnd || time.getHours() !== 0 || time.getMinutes() !== 0)
        {
            factor = (time.getHours() * 60 + time.getMinutes()) / (60 * 24)
        }
        return parent.width * factor
    }
}
