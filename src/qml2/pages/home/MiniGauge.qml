import QtQuick
import QtQuick.Layouts
import FluentUI

Rectangle {
    id: mini
    property string label: ""
    property string valueText: ""
    property real value: 0
    property color accent: FluTheme.primaryColor

    Layout.fillWidth: true
    Layout.preferredHeight: 44
    radius: 7
    color: FluTheme.dark ? Qt.rgba(1, 1, 1, 0.045) : Qt.rgba(0, 0, 0, 0.028)

    RowLayout {
        anchors { fill: parent; margins: 7 }
        spacing: 8
        Canvas {
            id: miniCanvas
            Layout.preferredWidth: 34
            Layout.preferredHeight: 34
            antialiasing: true
            onPaint: {
                var ctx = getContext("2d")
                ctx.clearRect(0, 0, width, height)
                var cx = width / 2
                var cy = height / 2
                var r = Math.min(width, height) / 2 - 4
                ctx.lineWidth = 4
                ctx.strokeStyle = FluTheme.dark ? "rgba(255,255,255,0.10)" : "rgba(0,0,0,0.07)"
                ctx.beginPath()
                ctx.arc(cx, cy, r, -Math.PI * 0.82, Math.PI * 0.82)
                ctx.stroke()
                ctx.strokeStyle = mini.accent
                ctx.beginPath()
                ctx.arc(cx, cy, r, -Math.PI * 0.82, -Math.PI * 0.82 + Math.PI * 1.64 * Math.max(0, Math.min(100, mini.value)) / 100)
                ctx.stroke()
            }
            Connections {
                target: mini
                function onValueChanged() { miniCanvas.requestPaint() }
                function onAccentChanged() { miniCanvas.requestPaint() }
            }
        }
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 0
            FluText { text: mini.label; font: FluTextStyle.Caption; color: FluTheme.fontSecondaryColor }
            FluText { text: mini.valueText; font: FluTextStyle.BodyStrong; Layout.fillWidth: true; elide: Text.ElideRight }
        }
    }
}
