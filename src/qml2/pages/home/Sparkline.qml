import QtQuick
import QtQuick.Layouts
import FluentUI

Canvas {
    id: spark
    property real value: 0
    property color accent: FluTheme.primaryColor
    property var samples: [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    antialiasing: true
    Layout.fillWidth: true
    Layout.preferredHeight: 30

    Timer {
        interval: 1100
        running: true
        repeat: true
        onTriggered: {
            var next = spark.samples.slice(1)
            next.push(Math.max(0, Math.min(100, spark.value)))
            spark.samples = next
            spark.requestPaint()
        }
    }

    onPaint: {
        var ctx = getContext("2d")
        ctx.clearRect(0, 0, width, height)
        ctx.strokeStyle = Qt.rgba(accent.r, accent.g, accent.b, 0.95)
        ctx.lineWidth = 2
        ctx.lineJoin = "round"
        ctx.beginPath()
        for (var i = 0; i < samples.length; i++) {
            var x = samples.length === 1 ? 0 : i * width / (samples.length - 1)
            var y = height - (samples[i] / 100) * (height - 6) - 3
            if (i === 0) ctx.moveTo(x, y)
            else ctx.lineTo(x, y)
        }
        ctx.stroke()
    }
}
