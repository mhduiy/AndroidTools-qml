import QtQuick
import QtQuick.Layouts
import FluentUI

Canvas {
    id: chart
    property real cpu: 0
    property real gpu: 0
    property real ram: 0
    property real temp: 0
    property real fps: 0
    property var cpuSamples: []
    property var gpuSamples: []
    property var ramSamples: []
    property var tempSamples: []
    property var fpsSamples: []
    readonly property int sampleCount: 34

    Layout.fillWidth: true
    Layout.preferredHeight: 58
    antialiasing: true

    function pushSample(values, nextValue) {
        var arr = values.slice(Math.max(0, values.length - sampleCount + 1))
        arr.push(Math.max(0, Math.min(100, nextValue)))
        while (arr.length < sampleCount)
            arr.unshift(0)
        return arr
    }

    Timer {
        interval: 1100
        running: true
        repeat: true
        onTriggered: {
            chart.cpuSamples = chart.pushSample(chart.cpuSamples, chart.cpu)
            chart.gpuSamples = chart.pushSample(chart.gpuSamples, chart.gpu)
            chart.ramSamples = chart.pushSample(chart.ramSamples, chart.ram)
            chart.tempSamples = chart.pushSample(chart.tempSamples, chart.temp)
            chart.fpsSamples = chart.pushSample(chart.fpsSamples, chart.fps)
            chart.requestPaint()
        }
    }

    function drawLine(ctx, samples, color) {
        ctx.strokeStyle = color
        ctx.lineWidth = 1.8
        ctx.lineJoin = "round"
        ctx.beginPath()
        for (var i = 0; i < samples.length; i++) {
            var x = samples.length <= 1 ? 0 : i * width / (samples.length - 1)
            var y = height - 10 - samples[i] * (height - 18) / 100
            if (i === 0) ctx.moveTo(x, y)
            else ctx.lineTo(x, y)
        }
        ctx.stroke()
    }

    onPaint: {
        var ctx = getContext("2d")
        ctx.clearRect(0, 0, width, height)
        ctx.strokeStyle = FluTheme.dark ? "rgba(255,255,255,0.08)" : "rgba(0,0,0,0.06)"
        ctx.lineWidth = 1
        for (var i = 0; i < 4; i++) {
            var y = 8 + i * (height - 16) / 3
            ctx.beginPath()
            ctx.moveTo(0, y)
            ctx.lineTo(width, y)
            ctx.stroke()
        }
        drawLine(ctx, cpuSamples, "#0f7b6c")
        drawLine(ctx, gpuSamples, "#2563eb")
        drawLine(ctx, ramSamples, "#7c3aed")
        drawLine(ctx, tempSamples, "#ca8a04")
        drawLine(ctx, fpsSamples, "#d83b01")
    }
}
