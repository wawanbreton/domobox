import QtQuick 2.0

Rectangle {
    id: dailyPeriodsItem
    color: domoStyle.cold

    required property var periods

    Rectangle {
        id: hotPeriod1
        x: timeToPosition(dailyPeriodsItem.periods.period1.begin, false)
        width: timeToPosition(dailyPeriodsItem.periods.period1.end, true) - x
        height: parent.height
        color: domoStyle.hot
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
