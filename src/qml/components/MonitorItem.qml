import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtCharts
import MFloat
import App

MWrapper {
    id: root
    property var cutValue: 0
    property var maxX: 100
    property var minX: 0
    property var maxY: 50
    property var minY: 0
    property var on: false
    Layout.preferredHeight: 200
    Layout.fillWidth: true
    title: "电池电量"
    titleRightContent: MSwitchButton {
        id: swicthBtn
        onStatusChanged: {
            root.on = status
        }
    }
    Rectangle {
        radius: 12
        border.width: 1
        border.color: "gray"
        color: "transparent"

        ChartView {
            id: chartView
            anchors.fill: parent
            antialiasing: true
            legend.visible: false
            animationOptions: ChartView.NoAnimation
            dropShadowEnabled: true
            backgroundColor: "transparent"
            plotAreaColor: "transparent"
            margins.left: 0
            margins.right: 0
            margins.top: 0
            margins.bottom: 0

            ValueAxis {
                id: xAxis
                min: root.minX
                max: root.maxX
                labelsVisible: false
                gridVisible: false
                color: App.windowTextColor
                labelsColor: App.windowTextColor
                labelsFont.bold: true
                titleVisible: false
                lineVisible: false
            }

            ValueAxis {
                id: yAxis
                min: root.minY
                max: root.maxY
                tickCount: 5 
                gridLineColor: App.borderColor
                color: App.windowTextColor
                labelsColor: App.windowTextColor
                labelsFont.bold: true
                labelsFont.pixelSize: 10
            }
            SplineSeries {
                id: splineSeries
                axisX: xAxis
                axisY: yAxis
            }

            Timer {
                id: dataTimer
                interval: 5000
                running: root.on
                repeat: true
                onTriggered: {
                    let x = splineSeries.count
                    let y = root.cutValue
                    if (x > root.maxX) {
                        root.maxX = x;
                    } else if (x < root.minX) {
                        root.minX = x;
                    }

                    if (x > 150) {
                        root.minX += 1
                    }

                    if (y > root.maxY) {
                        root.maxY = y + 1
                    } else if (y < root.minY) {
                        root.minY = y
                    }

                    splineSeries.append(x, y);
                }
            }
        }
    }
}
