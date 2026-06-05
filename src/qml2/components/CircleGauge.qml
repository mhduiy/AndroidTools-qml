import QtQuick
import FluentUI

Canvas {
    id: control

    property real value: 0
    property real minValue: 0
    property real maxValue: 100
    property string label: ""
    property string subtitle: ""
    property string unit: "%"
    property bool animate: true
    property real arcWidth: 8
    property int textSize: 18
    property int labelSize: 11

    property real displayValue: 0

    Behavior on displayValue {
        enabled: control.animate
        SmoothedAnimation {
            duration: 700
            velocity: -1
            easing.type: Easing.OutCubic
        }
    }

    onValueChanged: {
        if (control.animate) displayValue = value
        else { displayValue = value; requestPaint() }
    }
    onDisplayValueChanged: requestPaint()
    onWidthChanged: requestPaint()
    onHeightChanged: requestPaint()

    implicitWidth: 100
    implicitHeight: 100
    renderStrategy: Canvas.Cooperative
    antialiasing: true

    Component.onCompleted: displayValue = value

    function arcColor(v) {
        var t = (v - minValue) / Math.max(1, maxValue - minValue)
        t = Math.max(0, Math.min(1, t))
        return Qt.hsla(0.33 - t * 0.33, 0.85, 0.48, 1)
    }

    function formatValue(val) {
        if (unit === "GB" || unit === "V")
            return val.toFixed(1)
        return Math.round(val).toString()
    }

    onPaint: {
        var ctx = getContext("2d")
        var w = width, h = height
        var bottomPad = label ? 14 : (subtitle ? 10 : 0)
        var baseH = h - bottomPad
        var cx = w / 2, cy = baseH / 2
        var r = Math.min(w, baseH) / 2 - arcWidth - 4

        ctx.clearRect(0, 0, w, h)

        var pct = (displayValue - minValue) / Math.max(1, maxValue - minValue)
        pct = Math.max(0, Math.min(1, pct))
        var col = arcColor(displayValue)
        var dark = FluTheme.dark

        var startAngle = -225 * Math.PI / 180
        var endAngle = startAngle + 270 * Math.PI / 180

        // Track
        ctx.beginPath()
        ctx.arc(cx, cy, r, startAngle, endAngle)
        ctx.strokeStyle = dark ? Qt.rgba(1,1,1,0.06) : Qt.rgba(0,0,0,0.05)
        ctx.lineWidth = arcWidth
        ctx.lineCap = "round"
        ctx.stroke()

        if (pct > 0.003) {
            var sweep = startAngle + (endAngle - startAngle) * pct

            // Glow
            ctx.beginPath()
            ctx.arc(cx, cy, r, startAngle, sweep)
            ctx.strokeStyle = Qt.rgba(col.r, col.g, col.b, 0.15)
            ctx.lineWidth = arcWidth + 10
            ctx.lineCap = "round"
            ctx.stroke()

            // Arc with gradient
            ctx.beginPath()
            ctx.arc(cx, cy, r, startAngle, sweep)
            var grad = ctx.createLinearGradient(cx - r, cy, cx + r, cy)
            grad.addColorStop(0, Qt.lighter(col, 1.5))
            grad.addColorStop(1, col)
            ctx.strokeStyle = grad
            ctx.lineWidth = arcWidth
            ctx.lineCap = "round"
            ctx.stroke()
        }

        // Center value
        ctx.fillStyle = dark ? "#f0f0f0" : "#1a1a1a"
        ctx.font = "bold " + textSize + "px sans-serif"
        ctx.textAlign = "center"
        ctx.textBaseline = "middle"
        ctx.fillText(formatValue(displayValue) + unit, cx, cy - 2)

        // Label
        var labelY = baseH + 10
        if (label) {
            ctx.fillStyle = dark ? "#bbb" : "#555"
            ctx.font = labelSize + "px sans-serif"
            ctx.fillText(label, cx, labelY)
        }

        // Subtitle
        if (subtitle) {
            ctx.fillStyle = FluTheme.fontSecondaryColor
            ctx.font = "9px sans-serif"
            ctx.fillText(subtitle, cx, labelY + 12)
        }
    }
}
