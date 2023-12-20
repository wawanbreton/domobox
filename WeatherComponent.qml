import QtQuick 2.14
import QtQuick.Shapes 1.14

DomoComponent {
    id: domoComponent
    state: "collapsed"
    clip: true

    Item {
        id: itemCollapsed
        anchors.fill: parent
        visible: opacity > 0.0

        Image {
            id: image
            y: 0
            width: 100
            height: 100
            source: resource.weather.icon
            anchors.horizontalCenter: parent.horizontalCenter
            fillMode: Image.PreserveAspectFit
        }

        Text {
            id: textTemperature
            text: resource.weather.temperature
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: image.bottom
            font.pixelSize: domoStyle.fontSizeBig
            color: domoStyle.foreground
        }

        Column {
            id: forecast
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: textTemperature.bottom
            anchors.topMargin: 18
            anchors.bottom: parent.bottom

            Repeater {
                model: [1, 3, 7]

                Rectangle {
                    height: parent.height / 3
                    anchors.left: parent.left
                    anchors.right: parent.right
                    color: index % 2 == 0 ? domoStyle.componentBackgroundAlternative : domoStyle.componentBackground

                    Text {
                        text: resource.weather.forecast[modelData].hourStr
                        x: 4
                        anchors.verticalCenter: parent.verticalCenter
                        color: domoStyle.foreground
                        font.pixelSize: domoStyle.fontSizeMedium
                    }

                    Text {
                        text: resource.weather.forecast[modelData].temperatureStr
                        x: 62
                        anchors.verticalCenter: parent.verticalCenter
                        color: domoStyle.foreground
                        font.pixelSize: domoStyle.fontSizeMedium
                    }
                }
            }
        }
    }

    Item {
        id: itemExpanded
        anchors.fill: domoComponent

        Flickable {
            id: flickable
            anchors.fill: parent
            contentWidth: row.width

            Row {
                id: row

                Repeater {
                    id: repeaterWheather
                    model: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23]

                    property real minTemp
                    property real maxTemp

                    Item {
                        width: 60
                        height: flickable.height

                        Text {
                            id: textHour
                            text: resource.weather.forecast[modelData].hourStr
                            anchors.bottom: parent.bottom
                            anchors.bottomMargin: 20
                            width: parent.width
                            horizontalAlignment: Text.AlignHCenter
                            color: domoStyle.foreground
                            font.pixelSize: domoStyle.fontSizeSmall
                        }

                        Image {
                            id: imageWeather
                            source: resource.weather.forecast[modelData].icon
                            width: 44
                            fillMode: Image.PreserveAspectFit
                            anchors.bottom: textHour.top
                        }

                        Text {
                            id: textDetailedTemperature
                            text: resource.weather.forecast[modelData].temperatureStrShort
                            anchors.bottom: imageWeather.top
                            //anchors.bottomMargin: 20
                            width: parent.width
                            horizontalAlignment: Text.AlignHCenter
                            color: domoStyle.foreground
                            font.pixelSize: domoStyle.fontSizeSmall
                        }

                        Shape {
                            id: shape
                             width: parent.width
                             height: 150
                             anchors.bottom: textDetailedTemperature.top
                             anchors.bottomMargin: textHour.anchors.bottomMargin
                             anchors.top: parent.top
                             anchors.topMargin: textHour.anchors.bottomMargin
                             opacity: 0.5

                             property real temperatureHeightNm1: getTemperatureHeight(Math.max(modelData - 1, repeaterWheather.model[0]))
                             property real temperatureHeightN:   getTemperatureHeight(modelData)
                             property real temperatureHeightNp1: getTemperatureHeight(Math.min(modelData + 1, repeaterWheather.model[repeaterWheather.model.length - 1]))

                             ShapePath {
                                 strokeWidth: -1.0
                                 startX: 0
                                 startY: 0

                                 PathLine {
                                     x: 0;
                                     y: (shape.temperatureHeightNm1 + shape.temperatureHeightN) / 2.0
                                 }
                                 PathLine {
                                     x: shape.width / 2.0;
                                     y: shape.temperatureHeightN
                                 }
                                 PathLine {
                                     x: shape.width;
                                     y: (shape.temperatureHeightN + shape.temperatureHeightNp1) / 2.0
                                 }
                                 PathLine {
                                     x: shape.width;
                                     y: shape.height
                                 }
                                 PathLine {
                                     x: 0;
                                     y: shape.height
                                 }

                                 fillGradient: LinearGradient {
                                     x1: 0; y1: 0
                                     x2: 0; y2: shape.height
                                     GradientStop { position: 0; color: "#c0392b" }
                                     GradientStop { position: 0.9; color: "#2980b9" }
                                     GradientStop { position: 1.0; color: "transparent" }
                                 }
                             }

                             function getTemperatureHeight(index) {
                                 var max = repeaterWheather.maxTemp
                                 var min = repeaterWheather.minTemp
                                 var temperature = resource.weather.forecast[index].temperature
                                 var ratio = 1.0 - ((temperature - min) / (max - min))
                                 return shape.height * ratio
                             }
                         }
                    }
                }

                Connections {
                     target: resource.weather

                     function onDataChanged() {
                         var max = -1000
                         var min = 1000

                         var tempIndex
                         for(tempIndex of repeaterWheather.model)
                         {
                             var temperature = resource.weather.forecast[tempIndex].temperature
                             max = Math.max(max, temperature)
                             min = Math.min(min, temperature)
                         }

                         repeaterWheather.minTemp = min - 2
                         repeaterWheather.maxTemp = max
                     }
                 }
            }
        }

        onOpacityChanged: {
            if(opacity == 0.0)
            {
                flickable.contentX = flickable.originX
            }
        }
    }

    states: [
        State {
            name: "collapsed"

            PropertyChanges {
                target: itemCollapsed
                opacity: 1.0
            }

            PropertyChanges {
                target: itemExpanded
                opacity: 0.0
            }
        },

        State {
            name: "expanded"

            PropertyChanges {
                target: itemCollapsed
                opacity: 0.0
            }

            PropertyChanges {
                target: itemExpanded
                opacity: 1.0
            }
        }
    ]
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
