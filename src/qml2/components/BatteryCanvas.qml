import QtQuick
import QtQuick.Controls
import FluentUI

Canvas {
    id: control
    property int level: 0
    property bool charging: false

    property color fillColor: {
        if (level < 20) return Qt.rgba(0.9, 0.2, 0.2, 0.9)
        if (level < 50) return Qt.rgba(0.95, 0.55, 0.1, 0.9)
        if (level < 80) return Qt.rgba(0.15, 0.55, 0.9, 0.9)
        return Qt.rgba(0.15, 0.75, 0.35, 0.9)
    }

    property real waveOffset: 0

    NumberAnimation on waveOffset {
        running: charging
        from: 0
        to: Math.PI * 2
        duration: 2000
        loops: Animation.Infinite
    }

    onLevelChanged: requestPaint()
    onChargingChanged: requestPaint()
    onWaveOffsetChanged: requestPaint()

    renderStrategy: Canvas.Cooperative
    antialiasing: true

    onPaint: {
        var ctx = getContext("2d")
        var w = width, h = height
        ctx.clearRect(0, 0, w, h)

        var dark = FluTheme.dark
        var stroke = dark ? "#999" : "#555"

        // 电池外壳
        var bw = w * 0.5, bh = h * 0.58
        var bx = (w - bw) / 2, by = h * 0.28
        var br = 8

        ctx.strokeStyle = stroke
        ctx.lineWidth = 2.5
        ctx.lineJoin = "round"
        ctx.beginPath()
        ctx.moveTo(bx + br, by)
        ctx.lineTo(bx + bw - br, by)
        ctx.arcTo(bx + bw, by, bx + bw, by + br, br)
        ctx.lineTo(bx + bw, by + bh - br)
        ctx.arcTo(bx + bw, by + bh, bx + bw - br, by + bh, br)
        ctx.lineTo(bx + br, by + bh)
        ctx.arcTo(bx, by + bh, bx, by + bh - br, br)
        ctx.lineTo(bx, by + br)
        ctx.arcTo(bx, by, bx + br, by, br)
        ctx.stroke()

        // 正极触点
        ctx.fillStyle = stroke
        var tw = bw * 0.18, th = bh * 0.06
        ctx.fillRect(bx + (bw - tw) / 2, by - th, tw, th)

        // 液面
        var pad = 3
        var fillH = (bh - pad * 2) * (level / 100.0)
        if (fillH < 2) fillH = 2
        var fillY = by + bh - pad - fillH

        ctx.save()
        ctx.beginPath()
        ctx.rect(bx + pad, by + pad, bw - pad * 2, bh - pad * 2)
        ctx.clip()

        var grad = ctx.createLinearGradient(bx, fillY, bx, by + bh)
        grad.addColorStop(0, fillColor)
        grad.addColorStop(1, Qt.lighter(fillColor, 1.4))
        ctx.fillStyle = grad
        ctx.fillRect(bx + pad, fillY, bw - pad * 2, by + bh - pad - fillY)

        // 充电波纹
        if (charging && fillH > 8) {
            ctx.fillStyle = Qt.rgba(1, 1, 1, 0.18)
            ctx.beginPath()
            var cy = fillY + 4
            for (var x = bx + pad; x <= bx + bw - pad; x += 2) {
                var y = cy + Math.sin((x - bx) * 0.1 + waveOffset) * 3.5
                if (x === bx + pad) ctx.moveTo(x, y)
                else ctx.lineTo(x, y)
            }
            ctx.lineTo(bx + bw - pad, fillY)
            ctx.lineTo(bx + pad, fillY)
            ctx.closePath()
            ctx.fill()
        }
        ctx.restore()

        // 百分比文字
        ctx.fillStyle = dark ? "#fff" : "#222"
        ctx.font = "bold " + Math.round(bh * 0.32) + "px sans-serif"
        ctx.textAlign = "center"
        ctx.textBaseline = "middle"
        ctx.fillText(level + "%", w / 2, by + bh / 2)

        // 充电标记
        if (charging) {
            ctx.fillStyle = "#4fc3f7"
            ctx.font = Math.round(bh * 0.12) + "px sans-serif"
            ctx.fillText("CHARGING", w / 2, by + bh + 18)
        }
    }
}
