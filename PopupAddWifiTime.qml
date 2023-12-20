import QtQuick 2.0
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14
import QtQuick.Controls.Material 2.12

Popup {
    id: popup
    x: (parent.width - width) / 2.0
    y: (parent.height - height) / 2.0
    modal: true
    Material.background: domoStyle.componentBackground

    RowLayout {
        anchors.fill: parent
        spacing: 22

        Repeater {
             model: [10, 30, 60, 90, 120, 300]

             Button {
                 text: {
                     if(modelData < 60)
                     {
                         return "%1min".arg(modelData)
                     }
                     else
                     {
                         var hour = Math.floor(modelData / 60)
                         var minutes = modelData % 60
                         if(minutes)
                         {
                             return "%1h%2".arg(hour).arg(minutes)
                         }
                         else
                         {
                             return "%1h".arg(hour)
                         }
                     }
                 }

                 flat: true
                 font.pixelSize: domoStyle.fontSizeMedium
                 font.capitalization: Font.AllLowercase
                 icon.source: "/assets/time-%1.svg".arg(modelData)
                 icon.height: 82
                 icon.width: 82
                 icon.color: domoStyle.foreground
                 display: AbstractButton.TextUnderIcon
                 Material.foreground: domoStyle.foreground

                 onClicked: {
                     resource.wifi.addTime(modelData)
                     popup.close()
                 }
             }
         }
    }
}
