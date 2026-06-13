import QtQuick
import QtQuick.Layouts
import FluentUI

Canvas {
    id: chart

    property var fpsSamples: []
    property var cpuSamples: []
    property var gpuSamples: []
    property var ramSamples: []
    property var tempSamples: []
    property int sampleCount: 92

    Layout.fillWidth: true
    Layout.fillHeight: true
    antialiasing: true

    function push(values, nextValue) {
        var arr = values.slice(Math.max(0, values.length - sampleCount + 1))
        arr.push(Math.max(0, Math.min(100, nextValue)))
        while (arr.length < sampleCount)
            arr.unshift(0)
        return arr
    }

    function appendSample(fps, cpu, gpu, ram, temp) {
        fpsSamples = push(fpsSamples, fps)
        cpuSamples = push(cpuSamples, cpu)
        gpuSamples = push(gpuSamples, gpu)
        ramSamples = push(ramSamples, ram)
        tempSamples = push(tempSamples, temp)
        requestPaint()
    }

    function clear() {
        fpsSamples = []
        cpuSamples = []
        gpuSamples = []
        ramSamples = []
        tempSamples = []
        requestPaint()
    }

    function drawLine(ctx, samples, color, lineWidth) {
        if (samples.length <= 1)
            return
        ctx.strokeStyle = color
        ctx.lineWidth = lineWidth
        ctx.lineJoin = "round"
        ctx.lineCap = "round"
        ctx.beginPath()
        for (var i = 0; i < samples.length; i++) {
            var x = i * width / (samples.length - 1)
            var y = height - 24 - samples[i] * (height - 42) / 100
            if (i === 0) ctx.moveTo(x, y)
            else ctx.lineTo(x, y)
        }
        ctx.stroke()
    }

    onPaint: {
        var ctx = getContext("2d")
        ctx.clearRect(0, 0, width, height)

        ctx.fillStyle = FluTheme.dark ? "rgba(255,255,255,0.018)" : "rgba(0,0,0,0.012)"
        ctx.fillRect(0, 0, width, height)

        ctx.strokeStyle = FluTheme.dark ? "rgba(255,255,255,0.075)" : "rgba(0,0,0,0.070)"
        ctx.lineWidth = 1
        for (var i = 0; i <= 4; i++) {
            var y = 18 + i * (height - 42) / 4
            ctx.beginPath()
            ctx.moveTo(0, y)
            ctx.lineTo(width, y)
            ctx.stroke()
        }
        for (var x = 0; x <= width; x += width / 8) {
            ctx.beginPath()
            ctx.moveTo(x, 18)
            ctx.lineTo(x, height - 24)
            ctx.stroke()
        }

        drawLine(ctx, ramSamples, "#7c3aed", 1.7)
        drawLine(ctx, tempSamples, "#ca8a04", 1.8)
        drawLine(ctx, gpuSamples, "#2563eb", 1.9)
        drawLine(ctx, cpuSamples, "#0f7b6c", 2.0)
        drawLine(ctx, fpsSamples, "#d83b01", 2.2)

        ctx.fillStyle = FluTheme.dark ? "rgba(255,255,255,0.48)" : "rgba(0,0,0,0.46)"
        ctx.font = "11px Menlo"
        ctx.fillText("100", 4, 12)
        ctx.fillText("0", 4, height - 8)
    }
}
