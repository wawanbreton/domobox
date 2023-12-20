import QtQuick 2.14
import QtQuick.Shapes 1.1

Item {
    id: chart
    width: 640
    height: 480

    Item {
        anchors.fill: parent
        anchors.leftMargin: 10
        anchors.topMargin: 10
        anchors.rightMargin: 10
        anchors.bottomMargin: 10

        // Labels
        Item {
            id: labelsX

            anchors.bottom: parent.bottom
            anchors.left: axisY.horizontalCenter
            anchors.right: parent.right

            height: childrenRect.height

            Repeater {
                model: resource.temperature.gridDates

                Text {
                    color: domoStyle.foreground
                    font.pixelSize: domoStyle.fontSizeMedium
                    text: modelData.toLocaleTimeString("HH:mm")
                    x: dateToPosition(modelData) - width / 2
                }
            }
        }

        Item {
            id: labelsY

            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: axisX.verticalCenter

            width: childrenRect.width

            Repeater {
                model: resource.temperature.gridValues

                Text {
                    color: domoStyle.foreground
                    font.pixelSize: domoStyle.fontSizeMedium
                    text: modelData.toLocaleString(Qt.locale("fr_FR"), 'f', 1)
                    y: valueToPosition(modelData) - height / 2.0
                }
            }
        }

        // Tick marks
        Item {
            id: tickMarksX
            clip: true

            anchors.bottom: labelsX.top
            anchors.left: axisY.horizontalCenter
            anchors.right: parent.right
            height: 5

            Repeater {
                model: resource.temperature.gridDates

                Rectangle {
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    x: dateToPosition(modelData) - width / 2.0
                    width: 1
                    color: domoStyle.foreground
                }
            }
        }

        Item {
            id: tickMarksY
            clip: true

            anchors.left: labelsY.right
            anchors.bottom: axisX.verticalCenter
            anchors.top: parent.top
            width: tickMarksX.height

            Repeater {
                model: resource.temperature.gridValues

                Rectangle {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    y: valueToPosition(modelData) - height / 2.0
                    height: 1
                    color: domoStyle.foreground
                }
            }
        }

        // Axes
        Rectangle {
            id: axisX

            color: domoStyle.foreground
            height: 1
            anchors.bottom: tickMarksX.top
            anchors.left: axisY.horizontalCenter
            anchors.right: parent.right
        }

        Rectangle {
            id: axisY

            color: domoStyle.foreground
            width: 1
            anchors.top: parent.top
            anchors.bottom: axisX.verticalCenter
            anchors.left: tickMarksY.right
        }

        // Grids
        Item {
            anchors.left: axisY.horizontalCenter
            anchors.bottom: axisX.verticalCenter
            anchors.top: parent.top
            anchors.right: parent.right

            Repeater {
                model: resource.temperature.gridMinorDates

                Shape {
                    anchors.fill: parent
                    opacity: 0.4
                    asynchronous: true

                    ShapePath {
                         strokeColor: domoStyle.foreground
                         strokeWidth: 1.0
                         strokeStyle: ShapePath.DashLine
                         fillColor: "transparent"
                         capStyle: ShapePath.FlatCap

                         startX: dateToPosition(modelData)
                         startY: 0
                         PathLine { x: dateToPosition(modelData); y: parent.height }
                     }
                }
            }

            Repeater {
                model: resource.temperature.gridValues

                Shape {
                    anchors.fill: parent
                    opacity: 0.4
                    asynchronous: true

                    ShapePath {
                         strokeColor: domoStyle.foreground
                         strokeWidth: 1.0
                         strokeStyle: ShapePath.DashLine
                         fillColor: "transparent"
                         capStyle: ShapePath.FlatCap

                         startX: 0
                         startY: valueToPosition(modelData)
                         PathLine { x: parent.width; y: valueToPosition(modelData) }
                     }
                }
            }

            Shape {
                anchors.fill: parent
                asynchronous: true

                ShapePath {
                     strokeColor: domoStyle.foreground
                     strokeWidth: 3.0
                     capStyle: ShapePath.RoundCap
                     joinStyle: ShapePath.RoundJoin
                     fillColor: "transparent"

                     PathPolyline {
                         id: polyLine
                     }
                 }
            }
        }
    }

    onWidthChanged: rebuildChart()
    onHeightChanged: rebuildChart()

    Connections {
        target: resource.temperature

        function onTemperatureChanged() {
            rebuildChart()
        }
    }

    function rebuildChart() {
        var timestamps = resource.temperature.historyTimestamps
        var values = resource.temperature.historyValues
        var list = []

        for(var i = 0 ; i < timestamps.length; ++i) {
            list.push(Qt.point(dateToPosition(timestamps[i]),
                               valueToPosition(values[i])))
       }

        polyLine.path = list
    }

    function dateToPosition(date) {
        var totalDelta = resource.temperature.maxDate - resource.temperature.minDate
        var delta = date - resource.temperature.minDate
        return (delta / totalDelta) * axisX.width
    }

    function valueToPosition(value) {
        var totalDelta = resource.temperature.maxValue - resource.temperature.minValue
        var delta = value - resource.temperature.minValue
        return axisY.height - ((delta / totalDelta) * axisY.height)
    }
}
