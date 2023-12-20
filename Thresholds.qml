import QtQuick 2.0
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.0

Rectangle {
    required property var mainColor
    required property var thresholds

    Material.foreground: mainColor
    color: "transparent"
    border.color: mainColor
    border.width: 2.0

    GridLayout {
        columns: 2
        anchors.fill: parent
        anchors.margins: 8

        Text {
            text: "Min :"
            color: mainColor
            font.pixelSize: domoStyle.fontSizeSmall
            verticalAlignment: Text.AlignVCenter
        }

        SpinBox {
            Material.accent: mainColor
            Layout.fillWidth: true
            from: 100
            to: 300
            value: thresholds.min * 10

            Rectangle {
                anchors.fill: parent
                color: "transparent"
                border.width: 1.0
                border.color: mainColor
            }

            onValueChanged: thresholds.min = value / 10
            textFromValue: temperatureToString
            valueFromText: stringToTemperature
        }

        Text {
            text: "Max :"
            color: mainColor
            font.pixelSize: domoStyle.fontSizeSmall
        }

        SpinBox  {
            Material.accent: mainColor
            Layout.fillWidth: true
            from: 100
            to: 300
            value: thresholds.max * 10

            Rectangle {
                anchors.fill: parent
                color: "transparent"
                border.width: 1.0
                border.color: mainColor
            }

            onValueChanged: thresholds.max = value / 10
            textFromValue: temperatureToString
            valueFromText: stringToTemperature
        }
    }

    function temperatureToString(value, locale) {
        return "%1 °C".arg(Number(value / 10.0).toLocaleString(locale, 'f', 1));
    }

    function stringToTemperature(text, locale) {
        return Number.fromLocaleString(locale, text.substring(0, text.length - 3)) * 10.0;
    }
}
