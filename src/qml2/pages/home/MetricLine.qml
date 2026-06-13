import QtQuick
import QtQuick.Layouts
import FluentUI

ColumnLayout {
    id: metric
    property string name: ""
    property string value: ""
    property real percent: 0
    property color accent: FluTheme.primaryColor
    spacing: 4

    RowLayout {
        Layout.fillWidth: true
        FluText { text: metric.name; font: FluTextStyle.Caption; color: FluTheme.fontSecondaryColor; Layout.fillWidth: true }
        FluText { text: metric.value; font: FluTextStyle.BodyStrong }
    }
    Rectangle {
        Layout.fillWidth: true
        Layout.preferredHeight: 6
        radius: 3
        color: FluTheme.dark ? Qt.rgba(1, 1, 1, 0.08) : Qt.rgba(0, 0, 0, 0.055)
        Rectangle {
            height: parent.height
            radius: parent.radius
            width: Math.max(0, Math.min(parent.width, parent.width * metric.percent / 100))
            color: metric.accent
            Behavior on width { SmoothedAnimation { duration: 360 } }
        }
    }
}
