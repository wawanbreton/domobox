import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Shapes 1.15
import QtQuick.Controls.Material 2.12
import Domobox 1.0

DomoDialog {
    id: holiday

    readonly property int dayMs: 24 * 60 * 60 * 1000

    readonly property var initialDate: {
        var date = new Date()
        var deltaDays = 0
        if(date.getDay() === 0) // Sunday
        {
            deltaDays = -6
        }
        else
        {
            deltaDays = -date.getDay() + 1
        }
        date.setTime(date.getTime() + deltaDays * dayMs)

        date.setHours(0)
        date.setMinutes(0)
        date.setSeconds(0)
        date.setMilliseconds(0)
        return date
    }

    readonly property var rightDate: new Date(initialDate.getTime() + 7 * dayMs)

    readonly property var lastDate: new Date(rightDate.getTime() + 7 * 5 * dayMs)

    property var destinationDate: {
        var currentDate = new Date()
        var deltaDays = 7 - currentDate.getDay()
        var result = new Date(currentDate.getTime() + deltaDays * dayMs)
        result.setHours(16)
        result.setMinutes(0)
        result.setSeconds(0)
        result.setMilliseconds(0)
        return result
    }

    readonly property real dateRangeStartX: dateToX(new Date())

    readonly property real dateRangeWidth: dateToX(destinationDate) - dateRangeStartX

    Text {
        id: textTitle
        anchors.top: parent.top
        anchors.topMargin: domoStyle.margin - 8
        anchors.left: calendar.right
        anchors.leftMargin: domoStyle.margin
        text: "Retour :"
        color: domoStyle.foreground
        font.pixelSize: domoStyle.fontSizeMedium
        font.bold: true
    }

    Rectangle {
        id: calendar
        color: "transparent"
        border.color: domoStyle.foreground
        border.width: 1.5
        anchors.top: parent.top
        anchors.topMargin: domoStyle.margin
        anchors.left: parent.left
        anchors.leftMargin: domoStyle.margin
        anchors.bottom: buttonsTop
        anchors.bottomMargin: domoStyle.margin
        width: 350

        Column {
            anchors.fill: parent
            spacing: 0
            z: -2.0

            Repeater {
                model: 7

                Rectangle {
                    width: calendar.width
                    height: calendar.height / 7.0
                    color: index == 0 ? domoStyle.componentBackground :
                                        domoStyle.componentBackgroundAlternative
                }
            }
        }

        Row {
            anchors.fill: parent
            anchors.leftMargin: calendar.width / 7.0 - calendar.border.width / 2.0
            spacing: (calendar.width - 6 * calendar.border.width) / 7.0
            z: -1.0

            Repeater {
                model: 6

                Rectangle {
                    width: calendar.border.width
                    height: calendar.height
                    color: domoStyle.foreground
                }
            }
        }

        Column {
            anchors.fill: parent
            anchors.topMargin: calendar.height / 7.0 - calendar.border.width / 2.0
            spacing: (calendar.height - 6 * calendar.border.width) / 7.0
            z: -1.0

            Repeater {
                model: 6

                Rectangle {
                    width: calendar.width
                    height: calendar.border.width
                    color: domoStyle.foreground
                }
            }
        }

        Row {
            anchors.fill: parent
            spacing: 0

            Repeater {
                model: ["lun", "mar", "mer", "jeu", "ven", "sam", "dim"]

                Text {
                    width: calendar.width / 7.0
                    height: calendar.height / 7.0
                    color: domoStyle.foreground
                    font.pixelSize: domoStyle.fontSizeSmall
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    text: modelData
                }
            }
        }

        Column {
            id: columnDate
            z: -1.5

            anchors.fill: parent
            anchors.topMargin: calendar.height / 7.0
            spacing: 0

            Repeater {
                id: repeaterDateColumn
                model: 6

                Row {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    property int rowIndex: model.index

                    Repeater {
                        id: repeaterDateRow
                        model: 7

                        Item {
                            width: calendar.width / 7.0
                            height: calendar.height / 7.0

                            property var date: {
                                var deltaDays = rowIndex * 7 + index
                                var deltaMs = deltaDays * dayMs
                                return new Date(holiday.initialDate.getTime() + deltaMs)
                            }

                            Rectangle {
                                anchors.fill: parent
                                color: domoStyle.componentBackground
                                visible: parent.date.getDate() === new Date().getDate() &&
                                         parent.date.getMonth() === new Date().getMonth()
                            }

                            Text {
                                anchors.fill: parent

                                text: parent.date.getDate()

                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                font.pixelSize: domoStyle.fontSizeSmall
                                color: domoStyle.foreground
                            }
                        }
                    }
                }
            }
        }

        Column {
            id: columnArrow
            anchors.fill: parent
            anchors.topMargin: (calendar.height / 7.0)
            spacing: 0

            readonly property int arrowLineWidthHeight: 6
            readonly property int arrowWidth: 1.5 * arrowLineWidthHeight
            readonly property int arrowHeight: 3 * arrowLineWidthHeight

            Repeater {
                model: 6

                Item {
                    width: calendar.width
                    height: calendar.height / 7.0
                    clip: true

                    Rectangle {
                        id: rectangleArrowLine
                        x: holiday.dateRangeStartX - calendar.width * index
                        y: parent.height * 0.78
                        width: holiday.dateRangeWidth - columnArrow.arrowWidth
                        height: columnArrow.arrowLineWidthHeight
                        color: domoStyle.foreground

                    }

                    Shape {
                        id: shapeArrow
                        width: columnArrow.arrowWidth
                        height: columnArrow.arrowHeight
                        anchors.left: rectangleArrowLine.right
                        anchors.verticalCenter: rectangleArrowLine.verticalCenter

                        ShapePath {
                            strokeColor: "transparent"
                            fillColor: domoStyle.foreground
                            startX: 0; startY: 0
                            PathLine { x: 0; y: shapeArrow.height; }
                            PathLine { x: shapeArrow.width; y: shapeArrow.height / 2.0; }
                            PathLine { x: 0; y: 0; }
                        }
                    }
                }
            }
        }
    }

    Rectangle {
        color: "transparent"
        anchors.right: parent.right
        anchors.rightMargin: domoStyle.margin
        anchors.left: calendar.right
        anchors.leftMargin: domoStyle.margin
        anchors.top: textTitle.bottom
        anchors.topMargin: 4
        border.color: domoStyle.foreground
        border.width: calendar.border.width
        height: 170

        Row {
            id: rowDateSelector
            spacing: 0
            width: parent.width
            height: parent.height

            StyledTumbler {
                readonly property var dateStart: {
                    var date = new Date()
                    date.setHours(0)
                    date.setMinutes(0)
                    date.setSeconds(0)
                    date.setMilliseconds(0)
                    date.setTime(date.getTime() + dayMs)
                    return date
                }

               model: {
                   var result = []

                   for(var date = new Date(dateStart) ;
                       date < holiday.lastDate ;
                       date.setTime(date.getTime() + dayMs))
                   {
                       result.push(new Date(date))
                   }

                   return result
               }

               currentIndex: {
                   var delta = holiday.destinationDate.getTime() - dateStart.getTime()
                   var index = delta / dayMs
                   return (holiday.destinationDate.getTime() - dateStart.getTime()) / dayMs
               }
               width: 0.58 * parent.width
               height: parent.height
               wrap: false

               delegate: Component {
                   Label {
                       text: modelData.toLocaleDateString(Qt.locale(), "ddd dd/MM")
                       opacity: 1.0 - Math.abs(Tumbler.displacement) / (Tumbler.tumbler.visibleItemCount / 2)
                       horizontalAlignment: Text.AlignHCenter
                       verticalAlignment: Text.AlignVCenter
                       color: domoStyle.foreground
                       font.pixelSize: domoStyle.fontSizeSmall
                   }
               }

               onCurrentIndexChanged: {
                   var newDate = new Date(dateStart.getTime() + currentIndex * dayMs)
                   newDate.setHours(holiday.destinationDate.getHours())
                   holiday.destinationDate = newDate
               }
            }

            StyledTumbler {
                model: 24
                wrap: false
                currentIndex: destinationDate.getHours()
                width: 0.42 * parent.width
                height: parent.height

                delegate: Component {
                    Label {
                        text: formatTimeText(modelData)
                        opacity: 1.0 - Math.abs(Tumbler.displacement) / (Tumbler.tumbler.visibleItemCount / 2)
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        color: domoStyle.foreground
                        font.pixelSize: domoStyle.fontSizeSmall

                        function formatTimeText(value) {
                            var valueStr = value.toString()
                            valueStr = valueStr.length < 2 ? "0" + valueStr : valueStr;
                            valueStr += ":00"
                            return valueStr
                        }
                    }
                }

                onCurrentIndexChanged: {
                    var newDate = new Date(holiday.destinationDate)
                    newDate.setHours(currentIndex)
                    holiday.destinationDate = newDate
                }
            }
        }
    }

    Text {
        anchors.bottom: radioButtonModeKeep.top
        anchors.bottomMargin: -6
        anchors.left: radioButtonModeKeep.left
        font.pixelSize: domoStyle.fontSizeMedium
        font.bold: true
        color: domoStyle.foreground
        text: "Mode :"
    }

    RadioButton {
        id: radioButtonModeKeep
        text: "Maintien température"
        anchors.left: radioButtonModeStop.left
        anchors.bottom: radioButtonModeStop.top
        anchors.bottomMargin: -12
        anchors.right: radioButtonModeStop.right
        font.pixelSize: domoStyle.fontSizeMedium
        Material.accent: domoStyle.foreground
        Material.foreground: domoStyle.foreground
        checked: (destinationDate.getTime() - Date.now()) <= 2 * dayMs
    }

    RadioButton {
        id: radioButtonModeStop
        text: "Arrêt complet"
        anchors.left: calendar.right
        anchors.leftMargin: domoStyle.margin
        anchors.bottom: calendar.bottom
        anchors.bottomMargin: -16
        anchors.right: parent.right
        anchors.rightMargin: domoStyle.margin
        font.pixelSize: domoStyle.fontSizeMedium
        Material.accent: domoStyle.foreground
        Material.foreground: domoStyle.foreground
        checked: !radioButtonModeKeep.checked
    }

    function dateToX(date) {
        var delta = date.getTime() - initialDate.getTime()
        return (delta / (rightDate.getTime() - initialDate.getTime())) * calendar.width
    }

    onApply: {
        var mode = radioButtonModeKeep.checked ? FireMode.ShortHoliday : FireMode.LongHoliday
        resource.fire.startHoliday(destinationDate, mode)
    }
}
