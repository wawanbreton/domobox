import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.0
import QtQuick.Layouts 1.15
import QtQuick.Shapes 1.15
import QtGraphicalEffects 1.15
import Domobox 1.0

DomoDialog {
    TemperatureConfiguration {
        id: config

        Component.onCompleted: loadFromConfig()
    }

    Rectangle {
        id: rectangleDays
        anchors.left: parent.left
        anchors.leftMargin: domoStyle.margin
        anchors.top: parent.top
        anchors.topMargin: domoStyle.margin
        anchors.bottom: buttonsTop
        anchors.bottomMargin: domoStyle.margin
        width: 135
        color: "transparent"
        border.width: 2
        border.color: domoStyle.foreground

        ListView {
            id: listDays
            anchors.fill: parent
            model: ["Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi", "Dimanche"]
            interactive: false

            delegate: ItemDelegate {
                text: modelData
                width: parent.width
                height: listDays.height / listDays.model.length
                onClicked: listDays.currentIndex = index
                font.pixelSize: domoStyle.fontSizeSmall
                font.family: "Ubuntu"
                highlighted: ListView.isCurrentItem
                Material.foreground: domoStyle.foreground
            }

            onCurrentIndexChanged: { hotPeriod1.checked = false; hotPeriod2.checked = false; }
        }
    }

    Rectangle {
        id: rectanglePlanning
        anchors.left: rectangleDays.right
        anchors.leftMargin: domoStyle.margin
        anchors.top: rectangleDays.top
        anchors.right: parent.right
        anchors.rightMargin: domoStyle.margin
        anchors.bottom: thresholdsCold.top
        anchors.bottomMargin: domoStyle.margin
        color: "transparent"
        border.width: rectangleDays.border.width
        border.color: domoStyle.foreground

        Flickable {

            id: flickablePlanning
            width: parent.width
            height: 122
            clip: true
            contentWidth: itemPlanning.width
            contentX: width / 2.0

            Item {
                id: itemPlanningDetails
                height: 100
                width: rectanglePlanning.width

                Rectangle {
                    id: itemPlanning
                    color: domoStyle.cold
                    anchors.fill: parent

                    Repeater {
                        model: 23

                        Rectangle {
                            x: ((modelData + 1) / 24.0) * parent.width - width / 2.0
                            width: 1.5
                            color: domoStyle.coldOverlay
                            height: parent.height
                        }
                    }

                    /*Repeater {
                        model: 24

                        Shape {
                            x: ((modelData + 0.5) / 24.0) * parent.width - 0.75

                            ShapePath {
                                 strokeColor: domoStyle.coldOverlay
                                 strokeWidth: 1.5
                                 strokeStyle: ShapePath.DashLine
                                 fillColor: "transparent"
                                 capStyle: ShapePath.FlatCap

                                 startX: 0
                                 startY: 0
                                 PathLine { x: 0; y: parent.height }
                             }
                        }
                    }*/

                    Repeater {
                        model: 5

                        Text {
                            anchors.top: parent.bottom
                            x: ((((modelData + 1) * 4) / 24.0) * parent.width) - width / 2.0
                            text: {
                                var value = ((modelData + 1) * 4) % 24
                                return "%1%2:00".arg(value < 10 ? "0" : "").arg(value)
                            }
                            color: domoStyle.foreground
                            font.pixelSize: domoStyle.fontSizeSmall
                        }
                    }

                    ButtonGroup { id: hotPeriodsGroup }

                    HotPeriodItem {
                        id: hotPeriod1
                        height: parent.height
                        period: config.hotPeriods[listDays.currentIndex].period1
                        ButtonGroup.group: hotPeriodsGroup
                        checked: true
                    }

                    HotPeriodItem {
                        id: hotPeriod2
                        height: parent.height
                        period: config.hotPeriods[listDays.currentIndex].period2
                        ButtonGroup.group: hotPeriodsGroup
                    }
                }
            }
        }

        StyledButton {
            id: buttonRemove
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.top: flickablePlanning.bottom
            anchors.margins: 10
            icon.source: "/assets/minus.svg"
            enabled: config.hotPeriods[listDays.currentIndex].period2.active

            onClicked: config.hotPeriods[listDays.currentIndex].period2.active = false
        }

        StyledButton {
            id: buttonAdd
            anchors.right: buttonRemove.left
            anchors.rightMargin: buttonRemove.anchors.margins
            anchors.bottom: buttonRemove.bottom
            anchors.top: buttonRemove.top
            icon.source: "/assets/add.svg"
            enabled: !config.hotPeriods[listDays.currentIndex].period2.active

            onClicked: config.hotPeriods[listDays.currentIndex].period2.active = true
        }

        Item {
            anchors.left: parent.left
            anchors.leftMargin: buttonRemove.anchors.margins
            anchors.bottom: buttonAdd.bottom
            anchors.top: buttonRemove.top
            anchors.right: buttonAdd.left
            anchors.rightMargin: buttonRemove.anchors.margins

            HotPeriodTimeSpinBox {
                id: spinBoxStart
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                width: (parent.width - buttonRemove.anchors.margins) / 2.0
                enabled: hotPeriodsGroup.checkState == Qt.PartiallyChecked
                to: {
                    if(hotPeriodsGroup.checkedButton)
                    {
                        var time = hotPeriodsGroup.checkedButton.period.end
                        time.setTime(time.getTime() - 2 * (60 * 60 * 1000))
                        return getValue(time)
                    }
                    else
                    {
                        return 47
                    }
                }

                value: {
                    if(hotPeriodsGroup.checkedButton)
                    {
                        return getValue(hotPeriodsGroup.checkedButton.period.begin)
                    }
                    else
                    {
                        return 0
                    }
                }

                onValueModified: {
                    if(hotPeriodsGroup.checkedButton)
                    {
                        hotPeriodsGroup.checkedButton.period.begin = getDate()
                    }
                }
            }

            HotPeriodTimeSpinBox {
                id: spinBoxStop
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                width: spinBoxStart.width
                enabled: hotPeriodsGroup.checkState == Qt.PartiallyChecked

                from: {
                    if(hotPeriodsGroup.checkedButton)
                    {
                        var time = hotPeriodsGroup.checkedButton.period.begin
                        time.setTime(time.getTime() + 2 * (60 * 60 * 1000))
                        return getValue(time)
                    }
                    else
                    {
                        return 0
                    }
                }

                value: {
                    if(hotPeriodsGroup.checkedButton)
                    {
                        var time = hotPeriodsGroup.checkedButton.period.end
                        if(time.getHours() === 0 && time.getMinutes() === 0)
                        {
                            return 48
                        }
                        else
                        {
                            return getValue(hotPeriodsGroup.checkedButton.period.end)
                        }
                    }
                    else
                    {
                        return 0
                    }
                }

                onValueModified: {
                    if(hotPeriodsGroup.checkedButton)
                    {
                        hotPeriodsGroup.checkedButton.period.end = getDate()
                    }
                }
            }
        }
    }

    Thresholds {
        id: thresholdsCold
        anchors.left: rectanglePlanning.left
        anchors.bottom: rectangleDays.bottom
        width: (rectanglePlanning.width - domoStyle.margin) / 2.0
        height: 130
        mainColor: domoStyle.cold
        thresholds: config.thresholdsCold
    }

    Thresholds {
        id: thresholdsHot
        anchors.right: rectanglePlanning.right
        anchors.bottom: rectangleDays.bottom
        width: (rectanglePlanning.width - domoStyle.margin) / 2.0
        height: 130
        mainColor: domoStyle.hot
        thresholds: config.thresholdsHot
    }

    onApply: {
        resource.fire.setConfiguration(config)
    }
}
