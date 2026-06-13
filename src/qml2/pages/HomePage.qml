import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import FluentUI
import ConnectManager 1.0
import DeviceControl 1.0
import BatteryDisguise 1.0
import ResolutionControl 1.0
import FileTransfer 1.0
import InputText 1.0
import StartActivity 1.0
import MirrorScene 1.0
import Resource 1.0
import ScrcpyConfig 1.0
import ImageFrameItem 1.0
import ImageDetailTools 1.0
import SoftListModel 1.0
import AppDetailControl 1.0
import FastBootDeviceManager 1.0
import FlashTools 1.0
import ADBLog 1.0
import ADT 1.0
import SystemInfo 1.0
import "qrc:/qml2/components"

FluContentPage {
    id: page
    title: ""

    property var device: ConnectManager.cutADBDevice
    property bool mirrorActive: Resource.mirror > 0
    property string selectedPackage: ""
    property string selectedAppName: ""
    property string selectedAppVersion: ""
    property string selectedAppIcon: ""
    property string transferLocalPath: ""
    property string fastbootImagePath: ""
    property string flashZipPath: ""
    property real ramPct: SystemInfo.ramTotal > 0 ? Math.round(SystemInfo.ramUsage / SystemInfo.ramTotal * 100) : 0
    property real storagePct: SystemInfo.storageTotal > 0 ? Math.round(SystemInfo.storageUsed / SystemInfo.storageTotal * 100) : 0
    property real batteryTemp: device ? device.batteryTemperature : 0

    Timer {
        id: initTimer
        interval: 800
        onTriggered: if (page.device) SystemInfo.startPolling()
    }

    Connections {
        target: ConnectManager
        function onCutADBDeviceChanged() {
            if (page.device) initTimer.start()
            else SystemInfo.stopPolling()
        }
    }

    Connections {
        target: mirrorView
        function onCppGenerateEvents(request, data) {
            if (request === "MIRROR_START") Resource.mirror = 1
            else if (request === "MIRROR_FINISHED") Resource.mirror = 0
        }
    }

    Connections {
        target: AppDetailControl
        function onIconLoaded(packageName, iconBase64) {
            if (packageName === page.selectedPackage)
                page.selectedAppIcon = iconBase64
        }
    }

    Component.onCompleted: if (page.device) initTimer.start()
    Component.onDestruction: {
        SystemInfo.stopPolling()
        if (page.mirrorActive) Resource.qmlRequest("REQUEST_MIRROR_FINISH", "")
    }

    function localPath(url) {
        var path = String(url)
        if (path.indexOf("file://") === 0)
            return decodeURIComponent(path.substring(Qt.platform.os === "windows" ? 8 : 7))
        return path
    }

    function toggleMirror() {
        page.mirrorActive ? Resource.qmlRequest("REQUEST_MIRROR_FINISH", "") : Resource.qmlRequest("REQUEST_MIRROR_START", "")
    }

    function restartMirror() {
        if (!page.mirrorActive)
            return
        Resource.qmlRequest("REQUEST_MIRROR_FINISH", "")
        Resource.qmlRequest("REQUEST_MIRROR_START", "")
    }

    function openTool(index) {
        workbench.currentIndex = index
    }

    function selectApp(pkg, name, version, icon) {
        page.selectedPackage = pkg || ""
        page.selectedAppName = name || ""
        page.selectedAppVersion = version || ""
        page.selectedAppIcon = icon || ""
        if (page.selectedPackage.length > 0) {
            AppDetailControl.updateInfo(page.selectedPackage)
            if (page.selectedAppIcon.length === 0)
                AppDetailControl.requestLoadIcon(page.selectedPackage)
        }
    }

    function resolutionPart(index, fallbackValue) {
        if (!page.device || !page.device.resolution)
            return fallbackValue
        var parts = page.device.resolution.split("x")
        return parts[index] || fallbackValue
    }

    function batteryColor(level) {
        if (level < 20) return "#d83b01"
        if (level < 45) return "#ca8a04"
        return "#0f7b6c"
    }

    function chargingText(type) {
        if (type === ADT.AC) return "AC"
        if (type === ADT.USB) return "USB"
        if (type === ADT.Wireless) return "无线"
        if (type === ADT.Dock) return "Dock"
        return "未充电"
    }

    function batteryStatusText(status) {
        if (status === 2) return "充电中"
        if (status === 3) return "放电"
        if (status === 4) return "未充电"
        if (status === 5) return "已充满"
        return status > 0 ? String(status) : "--"
    }

    function batteryHealthText(health) {
        if (health === 2) return "良好"
        if (health === 3) return "过热"
        if (health === 4) return "损坏"
        if (health === 5) return "过压"
        if (health === 6) return "故障"
        if (health === 7) return "过冷"
        return health > 0 ? String(health) : "--"
    }

    Popup {
        id: fpsPopup
        width: 300
        implicitHeight: qualityContent.implicitHeight + padding * 2
        x: parent.width - width - 18
        y: 58
        padding: 14
        modal: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
        background: Rectangle {
            radius: 8
            color: FluTheme.dark ? Qt.rgba(0.10, 0.11, 0.13, 0.98) : Qt.rgba(1, 1, 1, 0.98)
            border.color: FluTheme.dividerColor
        }
        contentItem: ColumnLayout {
            id: qualityContent
            width: fpsPopup.availableWidth
            spacing: 9
            FluText { text: "投屏质量"; font: FluTextStyle.BodyStrong }
            GridLayout {
                columns: 2
                columnSpacing: 10
                rowSpacing: 8
                FluText { text: "帧率"; color: FluTheme.fontSecondaryColor }
                FluTextBox { id: fpsField; text: ScrcpyConfig.maxFps; Layout.fillWidth: true }
                FluText { text: "码率"; color: FluTheme.fontSecondaryColor }
                FluTextBox { id: brField; text: ScrcpyConfig.kBitRate; Layout.fillWidth: true }
            }
            RowLayout {
                Layout.fillWidth: true
                spacing: 6
                ActionButton { label: "省电"; icon: FluentIcons.QuietHours; dense: true; Layout.fillWidth: true; onPressed: { fpsField.text = "10"; brField.text = "2500" } }
                ActionButton { label: "均衡"; icon: FluentIcons.SpeedHigh; dense: true; Layout.fillWidth: true; onPressed: { fpsField.text = "24"; brField.text = "6000" } }
                ActionButton { label: "清晰"; icon: FluentIcons.Video; dense: true; Layout.fillWidth: true; onPressed: { fpsField.text = "30"; brField.text = "9000" } }
            }
            FluFilledButton {
                text: "应用"
                Layout.fillWidth: true
                onClicked: {
                    ScrcpyConfig.maxFps = parseInt(fpsField.text) || 30
                    ScrcpyConfig.kBitRate = parseInt(brField.text) || 8000
                    page.restartMirror()
                    fpsPopup.close()
                }
            }
        }
    }

    component Panel: Rectangle {
        property alias content: body.data
        radius: 8
        color: FluTheme.dark ? Qt.rgba(0.10, 0.11, 0.13, 0.76) : Qt.rgba(1, 1, 1, 0.78)
        border.width: 1
        border.color: FluTheme.dark ? Qt.rgba(1, 1, 1, 0.10) : Qt.rgba(0, 0, 0, 0.11)
        layer.enabled: true

        Item {
            id: body
            anchors.fill: parent
        }
    }

    component Header: RowLayout {
        property string title: ""
        property string subtitle: ""
        spacing: 7
        FluText { text: parent.title; font: FluTextStyle.BodyStrong }
        FluText { text: parent.subtitle; font: FluTextStyle.Caption; color: FluTheme.fontSecondaryColor; elide: Text.ElideRight; Layout.fillWidth: true }
    }

    component ActionButton: Rectangle {
        id: button
        property string label: ""
        property int icon: 0
        property color accent: FluTheme.primaryColor
        property bool dense: false
        signal pressed()

        Layout.preferredHeight: dense ? 30 : 34
        radius: 7
        color: enabled
               ? (mouse.containsMouse ? Qt.rgba(accent.r, accent.g, accent.b, FluTheme.dark ? 0.24 : 0.14)
                                      : (FluTheme.dark ? Qt.rgba(1, 1, 1, 0.055) : Qt.rgba(0, 0, 0, 0.035)))
               : Qt.rgba(0.5, 0.5, 0.5, 0.08)
        border.width: 1
        border.color: mouse.containsMouse ? accent : (FluTheme.dark ? Qt.rgba(1,1,1,0.11) : Qt.rgba(0,0,0,0.10))
        opacity: enabled ? 1 : 0.42

        Behavior on color { ColorAnimation { duration: 120 } }
        Behavior on border.color { ColorAnimation { duration: 120 } }

        RowLayout {
            anchors.centerIn: parent
            spacing: 5
            FluIcon { iconSource: button.icon; iconSize: button.dense ? 13 : 15; iconColor: button.accent; visible: button.icon > 0 }
            FluText { text: button.label; font: button.dense ? FluTextStyle.Caption : FluTextStyle.Body; color: FluTheme.fontPrimaryColor }
        }

        MouseArea {
            id: mouse
            anchors.fill: parent
            hoverEnabled: true
            enabled: button.enabled
            onClicked: button.pressed()
        }
    }

    component IconKey: Rectangle {
        id: key
        property string label: ""
        property int icon: 0
        property color accent: FluTheme.primaryColor
        signal pressed()

        Layout.preferredWidth: 62
        Layout.preferredHeight: 46
        radius: 8
        color: mouse.containsMouse ? Qt.rgba(accent.r, accent.g, accent.b, FluTheme.dark ? 0.25 : 0.14)
                                   : (FluTheme.dark ? Qt.rgba(1,1,1,0.045) : Qt.rgba(0,0,0,0.030))
        border.color: mouse.containsMouse ? accent : (FluTheme.dark ? Qt.rgba(1,1,1,0.10) : Qt.rgba(0,0,0,0.09))
        border.width: 1
        opacity: enabled ? 1 : 0.4

        ColumnLayout {
            anchors.centerIn: parent
            spacing: 2
            FluIcon { iconSource: key.icon; iconSize: 16; iconColor: key.accent; Layout.alignment: Qt.AlignHCenter }
            FluText { text: key.label; font: FluTextStyle.Caption; Layout.alignment: Qt.AlignHCenter }
        }

        MouseArea {
            id: mouse
            anchors.fill: parent
            hoverEnabled: true
            enabled: key.enabled
            onClicked: key.pressed()
        }
    }

    component MetricLine: ColumnLayout {
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

    component SpecRow: RowLayout {
        property string name: ""
        property string value: ""
        spacing: 8
        FluText { text: parent.name; font: FluTextStyle.Caption; color: FluTheme.fontSecondaryColor; Layout.preferredWidth: 48 }
        FluText { text: parent.value; font: FluTextStyle.BodyStrong; elide: Text.ElideRight; Layout.fillWidth: true }
    }

    component DataTile: Rectangle {
        id: tile
        property string label: ""
        property string value: ""
        property string sub: ""
        property color accent: FluTheme.primaryColor

        Layout.fillWidth: true
        Layout.preferredHeight: 50
        radius: 7
        color: FluTheme.dark ? Qt.rgba(1, 1, 1, 0.045) : Qt.rgba(0, 0, 0, 0.028)
        border.width: 1
        border.color: Qt.rgba(accent.r, accent.g, accent.b, FluTheme.dark ? 0.24 : 0.18)

        ColumnLayout {
            anchors { fill: parent; leftMargin: 10; rightMargin: 10; topMargin: 6; bottomMargin: 6 }
            spacing: 0
            RowLayout {
                Layout.fillWidth: true
                Rectangle { Layout.preferredWidth: 5; Layout.preferredHeight: 5; radius: 3; color: tile.accent }
                FluText { text: tile.label; font: FluTextStyle.Caption; color: FluTheme.fontSecondaryColor; Layout.fillWidth: true; elide: Text.ElideRight }
            }
            FluText { text: tile.value; font: FluTextStyle.BodyStrong; Layout.fillWidth: true; elide: Text.ElideRight }
            FluText { text: tile.sub; font: FluTextStyle.Caption; color: FluTheme.fontSecondaryColor; visible: tile.sub.length > 0; Layout.fillWidth: true; elide: Text.ElideRight }
        }
    }

    component CompactTile: Rectangle {
        id: compact
        property string label: ""
        property string value: ""
        property color accent: FluTheme.primaryColor

        Layout.fillWidth: true
        Layout.preferredHeight: 32
        radius: 7
        color: FluTheme.dark ? Qt.rgba(1, 1, 1, 0.040) : Qt.rgba(0, 0, 0, 0.024)
        border.width: 1
        border.color: Qt.rgba(accent.r, accent.g, accent.b, FluTheme.dark ? 0.18 : 0.13)

        RowLayout {
            anchors { fill: parent; leftMargin: 8; rightMargin: 8 }
            spacing: 7
            Rectangle { Layout.preferredWidth: 5; Layout.preferredHeight: 5; radius: 3; color: compact.accent }
            FluText { text: compact.label; font: FluTextStyle.Caption; color: FluTheme.fontSecondaryColor; Layout.preferredWidth: 64; elide: Text.ElideRight }
            FluText { text: compact.value; font: FluTextStyle.Caption; Layout.fillWidth: true; elide: Text.ElideRight }
        }
    }

    component LegendPill: RowLayout {
        property string label: ""
        property string value: ""
        property color accent: FluTheme.primaryColor
        spacing: 4
        Rectangle { Layout.preferredWidth: 6; Layout.preferredHeight: 6; radius: 3; color: parent.accent }
        FluText { text: parent.label + " " + parent.value; font: FluTextStyle.Caption; color: FluTheme.fontSecondaryColor }
    }

    component AppIconBox: Rectangle {
        id: appIcon
        property string source: ""
        property string title: ""
        property color accent: FluTheme.primaryColor

        radius: 8
        color: Qt.rgba(accent.r, accent.g, accent.b, FluTheme.dark ? 0.18 : 0.10)
        border.width: 1
        border.color: Qt.rgba(accent.r, accent.g, accent.b, FluTheme.dark ? 0.26 : 0.18)
        clip: true

        Image {
            anchors.fill: parent
            anchors.margins: 3
            source: appIcon.source
            fillMode: Image.PreserveAspectFit
            visible: source.length > 0
            asynchronous: true
        }
        FluText {
            anchors.centerIn: parent
            text: appIcon.title.length > 0 ? appIcon.title.charAt(0).toUpperCase() : "?"
            font: FluTextStyle.BodyStrong
            visible: appIcon.source.length === 0
            color: appIcon.accent
        }
    }

    component ActionCard: Rectangle {
        id: action
        property string title: ""
        property string subtitle: ""
        property int icon: 0
        property color accent: FluTheme.primaryColor
        signal pressed()

        Layout.fillWidth: true
        Layout.preferredHeight: 48
        radius: 7
        color: mouse.containsMouse ? Qt.rgba(accent.r, accent.g, accent.b, FluTheme.dark ? 0.18 : 0.10)
                                   : (FluTheme.dark ? Qt.rgba(1, 1, 1, 0.04) : Qt.rgba(0, 0, 0, 0.024))
        border.width: 1
        border.color: mouse.containsMouse ? accent : Qt.rgba(accent.r, accent.g, accent.b, FluTheme.dark ? 0.20 : 0.14)
        opacity: enabled ? 1 : 0.42

        RowLayout {
            anchors { fill: parent; margins: 8 }
            spacing: 8
            FluIcon { iconSource: action.icon; iconSize: 17; iconColor: action.accent; Layout.preferredWidth: 20 }
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 0
                FluText { text: action.title; font: FluTextStyle.Caption; Layout.fillWidth: true; elide: Text.ElideRight }
                FluText { text: action.subtitle; font: FluTextStyle.Caption; color: FluTheme.fontSecondaryColor; Layout.fillWidth: true; elide: Text.ElideRight }
            }
        }

        MouseArea {
            id: mouse
            anchors.fill: parent
            hoverEnabled: true
            enabled: action.enabled
            onClicked: action.pressed()
        }
    }

    component MiniGauge: Rectangle {
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

    component Sparkline: Canvas {
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

    component MultiLineChart: Canvas {
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

    Rectangle {
        anchors.fill: parent
        color: "transparent"

        ColumnLayout {
            anchors { fill: parent; margins: 10 }
            spacing: 8

            RowLayout {
                Layout.fillWidth: true
                Layout.preferredHeight: 44
                spacing: 8

                Panel {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    RowLayout {
                        anchors { fill: parent; leftMargin: 14; rightMargin: 10 }
                        spacing: 10

                        Rectangle {
                            Layout.preferredWidth: 9
                            Layout.preferredHeight: 9
                            radius: 5
                            color: page.device ? "#22c55e" : "#d83b01"
                            SequentialAnimation on opacity {
                                running: page.device
                                loops: Animation.Infinite
                                NumberAnimation { to: 0.35; duration: 900 }
                                NumberAnimation { to: 1; duration: 900 }
                            }
                        }

                        ColumnLayout {
                            spacing: 0
                            Layout.fillWidth: true
                            FluText {
                                text: page.device ? ((page.device.manufacturer || "") + " " + (page.device.model || page.device.deviceCode || "Android 设备")) : "等待设备连接"
                                font: FluTextStyle.BodyStrong
                                elide: Text.ElideRight
                                Layout.fillWidth: true
                            }
                            FluText {
                                text: page.device ? ((page.device.currentPackage || "未读取前台应用") + "  /  " + (page.device.serialNumber || page.device.deviceCode || "-")) : "连接设备后会自动汇总投屏、性能、应用和调试入口"
                                font: FluTextStyle.Caption
                                color: FluTheme.fontSecondaryColor
                                elide: Text.ElideRight
                                Layout.fillWidth: true
                            }
                        }

                        ActionButton { label: "刷新"; icon: FluentIcons.Refresh; dense: true; Layout.preferredWidth: 76; onPressed: ConnectManager.startCheckDevice() }
                        ActionButton { label: page.mirrorActive ? "停止投屏" : "投屏"; icon: FluentIcons.Video; dense: true; Layout.preferredWidth: 96; enabled: !!page.device; accent: "#0f7b6c"; onPressed: page.toggleMirror() }
                        ActionButton { label: "截图"; icon: FluentIcons.Camera; dense: true; Layout.preferredWidth: 76; enabled: !!page.device; onPressed: ImageDetailTools.shotScreen("") }
                    }
                }
            }

            RowLayout {
                Layout.fillWidth: true
                Layout.preferredHeight: 382
                Layout.maximumHeight: 382
                spacing: 8

                Panel {
                    Layout.preferredWidth: 276
                    Layout.fillHeight: true
                    ColumnLayout {
                        anchors { fill: parent; margins: 10 }
                        spacing: 7

                        Header { title: "设备档案"; subtitle: page.device ? (page.device.brand || page.device.deviceName || "") : "未连接" }

                        RowLayout {
                            Layout.fillWidth: true
                            spacing: 8
                            BatteryCanvas {
                                Layout.preferredWidth: 70
                                Layout.preferredHeight: 98
                                level: page.device ? page.device.batteryLevel : 0
                                charging: page.device ? page.device.chargingType !== ADT.None : false
                                showChargingText: false
                            }
                            ColumnLayout {
                                Layout.fillWidth: true
                                spacing: 6
                                MetricLine { name: "电量"; value: page.device ? page.device.batteryLevel + "%" : "--"; percent: page.device ? page.device.batteryLevel : 0; accent: page.batteryColor(page.device ? page.device.batteryLevel : 0) }
                                MetricLine { name: "温度"; value: page.batteryTemp.toFixed(1) + "C"; percent: Math.min(100, Math.max(0, (page.batteryTemp - 20) * 2)); accent: page.batteryTemp > 44 ? "#d83b01" : "#0f7b6c" }
                                MetricLine { name: "电压"; value: page.device ? (page.device.batteryVoltage / 1000.0).toFixed(2) + "V" : "--"; percent: page.device ? Math.min(100, page.device.batteryVoltage / 50) : 0; accent: "#2563eb" }
                            }
                        }

                        GridLayout {
                            Layout.fillWidth: true
                            columns: 2
                            columnSpacing: 6
                            rowSpacing: 6
                            DataTile { label: "系统"; value: page.device ? "Android " + (page.device.androidVersion || "-") : "-"; sub: page.device ? "SDK " + (page.device.sdkVersion || "-") : ""; accent: "#2563eb" }
                            DataTile { label: "屏幕"; value: page.device ? (page.device.resolution || "-") : "-"; sub: page.device ? (page.device.dpi || "-") + " DPI" : ""; accent: "#7c3aed" }
                            DataTile { label: "CPU"; value: page.device ? (page.device.maxCoreNum || "-") + " 核" : "-"; sub: page.device ? (page.device.maxFreq || "-") : ""; accent: "#0f7b6c" }
                            DataTile { label: "内存"; value: page.device ? (page.device.memory || "--") : "--"; sub: SystemInfo.ramTotal > 0 ? SystemInfo.ramTotal.toFixed(1) + " GB runtime" : ""; accent: "#64748b" }
                            DataTile { label: "电池"; value: page.device ? page.chargingText(page.device.chargingType) : "--"; sub: page.device ? page.batteryStatusText(page.device.batteryStatus) + " / " + page.batteryHealthText(page.device.batteryHealth) : ""; accent: page.batteryColor(page.device ? page.device.batteryLevel : 0) }
                            DataTile { label: "电流"; value: page.device ? page.device.batteryCurrent + " mA" : "--"; sub: SystemInfo.batteryTechnology || "scale " + (page.device ? page.device.batteryScale : "--"); accent: "#ca8a04" }
                            DataTile { label: "IP"; value: page.device ? (page.device.ipAddr || "未知") : "-"; sub: page.device ? (page.device.macAddr || "MAC --") : ""; accent: "#2563eb" }
                            DataTile { label: "代号"; value: page.device ? (page.device.deviceCode || "-") : "-"; sub: page.device ? (page.device.serialNumber || "-") : ""; accent: "#64748b" }
                        }

                        Item { Layout.fillHeight: true }

                        ActionButton {
                            label: "重启到 Fastboot"
                            icon: FluentIcons.DeveloperTools
                            enabled: !!page.device
                            Layout.fillWidth: true
                            accent: "#ca8a04"
                            onPressed: DeviceControl.control(ADT.Key, ADT.RebootToFB)
                        }
                    }
                }

                Panel {
                    Layout.preferredWidth: 454
                    Layout.fillHeight: true
                    ColumnLayout {
                        anchors { fill: parent; margins: 12 }
                        spacing: 8

                        RowLayout {
                            Layout.fillWidth: true
                            Header { title: "投屏舞台"; subtitle: page.mirrorActive ? "实时画面" : "待启动"; Layout.fillWidth: true }
                            ActionButton { label: "质量"; icon: FluentIcons.PlayerSettings; dense: true; Layout.preferredWidth: 72; enabled: !!page.device; onPressed: fpsPopup.open() }
                        }

                        RowLayout {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 288
                            spacing: 8

                            Rectangle {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                radius: 10
                                color: "#050608"
                                border.width: 1
                                border.color: FluTheme.dark ? Qt.rgba(1, 1, 1, 0.12) : Qt.rgba(0, 0, 0, 0.18)
                                clip: true

                                MirrorScene {
                                    id: mirrorView
                                    anchors { fill: parent; margins: 2 }
                                }

                                ImageFrameItem {
                                    anchors { fill: parent; margins: 2 }
                                    imageFrame: mirrorView.image
                                    hasAlphaChannel: false
                                }

                                Rectangle {
                                    visible: !page.mirrorActive
                                    anchors.fill: parent
                                    color: "#08090c"
                                    ColumnLayout {
                                        anchors.centerIn: parent
                                        spacing: 8
                                        FluIcon { iconSource: FluentIcons.CellPhone; iconSize: 34; iconColor: "#64748b"; Layout.alignment: Qt.AlignHCenter }
                                        FluText { text: page.device ? "投屏未启动" : "未连接设备"; color: "#94a3b8"; font: FluTextStyle.BodyStrong; Layout.alignment: Qt.AlignHCenter }
                                        FluText { text: page.device ? "顶部按钮可启动投屏" : "连接后显示完整画面"; color: "#64748b"; font: FluTextStyle.Caption; Layout.alignment: Qt.AlignHCenter }
                                    }
                                }
                            }

                            GridLayout {
                                Layout.preferredWidth: 132
                                Layout.fillHeight: true
                                columns: 2
                                columnSpacing: 6
                                rowSpacing: 6
                                IconKey { label: "主页"; icon: FluentIcons.Home; enabled: !!page.device; onPressed: DeviceControl.control(ADT.Key, ADT.Home) }
                                IconKey { label: "返回"; icon: FluentIcons.Back; enabled: !!page.device; onPressed: DeviceControl.control(ADT.Key, ADT.Back) }
                                IconKey { label: "电源"; icon: FluentIcons.PowerButton; enabled: !!page.device; accent: "#ca8a04"; onPressed: DeviceControl.control(ADT.Key, ADT.Power) }
                                IconKey { label: "菜单"; icon: FluentIcons.GlobalNavButton; enabled: !!page.device; onPressed: DeviceControl.control(ADT.Key, ADT.Menu) }
                                IconKey { label: "音量+"; icon: FluentIcons.Volume; enabled: !!page.device; onPressed: DeviceControl.control(ADT.Music, ADT.VolumeAdd) }
                                IconKey { label: "音量-"; icon: FluentIcons.Volume; enabled: !!page.device; onPressed: DeviceControl.control(ADT.Music, ADT.VolumeReduce) }
                                IconKey { label: "静音"; icon: FluentIcons.Mute; enabled: !!page.device; onPressed: DeviceControl.control(ADT.Key, ADT.Mute) }
                                IconKey { label: "重启"; icon: FluentIcons.Refresh; enabled: !!page.device; accent: "#d83b01"; onPressed: DeviceControl.control(ADT.Key, ADT.Reboot) }
                            }
                        }

                        RowLayout {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 34
                            spacing: 6
                            ActionButton { label: "控制"; icon: FluentIcons.Permissions; dense: true; Layout.fillWidth: true; onPressed: page.openTool(0) }
                            ActionButton { label: "文件"; icon: FluentIcons.Document; dense: true; Layout.fillWidth: true; onPressed: page.openTool(2) }
                            ActionButton { label: "应用"; icon: FluentIcons.AllApps; dense: true; Layout.fillWidth: true; onPressed: page.openTool(3) }
                            ActionButton { label: "刷机"; icon: FluentIcons.DeveloperTools; dense: true; Layout.fillWidth: true; accent: "#ca8a04"; onPressed: page.openTool(4) }
                            ActionButton { label: "日志"; icon: FluentIcons.ReportDocument; dense: true; Layout.fillWidth: true; onPressed: page.openTool(5) }
                        }
                    }
                }

                Panel {
                    Layout.preferredWidth: 330
                    Layout.fillHeight: true
                    ColumnLayout {
                        anchors { fill: parent; margins: 10 }
                        spacing: 7

                        Header { title: "实时状态"; subtitle: SystemInfo.polling ? "采集中" : "等待数据" }

                        GridLayout {
                            Layout.fillWidth: true
                            columns: 2
                            columnSpacing: 6
                            rowSpacing: 6
                            MiniGauge { label: "CPU 使用率"; value: SystemInfo.cpuUsage; valueText: SystemInfo.polling ? Math.round(SystemInfo.cpuUsage) + "%" : "--"; accent: "#0f7b6c" }
                            MiniGauge { label: "GPU 使用率"; value: SystemInfo.gpuUsage; valueText: SystemInfo.polling ? Math.round(SystemInfo.gpuUsage) + "%" : "--"; accent: "#2563eb" }
                            MiniGauge { label: "RAM 使用率"; value: page.ramPct; valueText: SystemInfo.ramTotal > 0 ? page.ramPct + "%" : "--"; accent: "#7c3aed" }
                            MiniGauge { label: "CPU 温度"; value: Math.min(100, Math.max(0, SystemInfo.cpuTemp)); valueText: SystemInfo.polling ? SystemInfo.cpuTemp.toFixed(1) + "C" : "--"; accent: SystemInfo.cpuTemp > 70 ? "#d83b01" : "#ca8a04" }
                        }

                        RowLayout {
                            Layout.fillWidth: true
                            spacing: 8
                            Header { title: "趋势"; subtitle: ""; Layout.fillWidth: true }
                            FluText { text: SystemInfo.fps > 0 ? SystemInfo.fps + " FPS" : "-- FPS"; font: FluTextStyle.Caption; color: FluTheme.fontSecondaryColor }
                        }

                        RowLayout {
                            Layout.fillWidth: true
                            spacing: 8
                            LegendPill { label: "CPU"; value: Math.round(SystemInfo.cpuUsage) + "%"; accent: "#0f7b6c" }
                            LegendPill { label: "RAM"; value: page.ramPct + "%"; accent: "#7c3aed" }
                            LegendPill { label: "TEMP"; value: SystemInfo.cpuTemp.toFixed(0) + "C"; accent: "#ca8a04" }
                        }

                        MultiLineChart {
                            cpu: SystemInfo.cpuUsage
                            gpu: SystemInfo.gpuUsage
                            ram: page.ramPct
                            temp: Math.min(100, Math.max(0, SystemInfo.cpuTemp))
                            fps: Math.min(100, Math.max(0, SystemInfo.fps / 60 * 100))
                        }

                        GridLayout {
                            Layout.fillWidth: true
                            columns: 2
                            columnSpacing: 6
                            rowSpacing: 6
                            CompactTile { label: "RAM"; value: SystemInfo.ramTotal > 0 ? SystemInfo.ramUsage.toFixed(1) + "/" + SystemInfo.ramTotal.toFixed(1) + "GB" : "--"; accent: "#7c3aed" }
                            CompactTile { label: "存储"; value: SystemInfo.storageTotal > 0 ? SystemInfo.storageUsed.toFixed(0) + "/" + SystemInfo.storageTotal.toFixed(0) + "GB" : "--"; accent: page.storagePct > 85 ? "#d83b01" : "#2563eb" }
                            CompactTile { label: "前台"; value: SystemInfo.foregroundPackage || (page.device && page.device.currentPackage ? page.device.currentPackage : "--"); accent: "#0f7b6c" }
                            CompactTile { label: "Activity"; value: SystemInfo.foregroundActivity || (page.device && page.device.currentActivity ? page.device.currentActivity : "--"); accent: "#0f7b6c" }
                            CompactTile { label: "进程"; value: SystemInfo.foregroundPid > 0 ? "PID " + SystemInfo.foregroundPid + " / " + SystemInfo.foregroundMemoryMB.toFixed(0) + "MB" : "--"; accent: "#64748b" }
                            CompactTile { label: "投屏"; value: page.mirrorActive ? "运行中" : "未启动"; accent: page.mirrorActive ? "#0f7b6c" : "#64748b" }
                        }
                    }
                }
            }

            Panel {
                Layout.fillWidth: true
                Layout.preferredHeight: 228
                Layout.maximumHeight: 228

                    FluPivot {
                        id: workbench
                        anchors { fill: parent; margins: 10 }
                        headerHeight: 32
                        font.pixelSize: 13

                    FluPivotItem {
                        title: "控制"
                        contentItem: Component {
                            GridLayout {
                                anchors { fill: parent; margins: 10 }
                                columns: 3
                                columnSpacing: 12
                                rowSpacing: 10

                                ColumnLayout {
                                    Layout.fillWidth: true
                                    spacing: 7
                                    Header { title: "媒体"; subtitle: "播放与音量" }
                                    GridLayout {
                                        Layout.fillWidth: true
                                        columns: 2
                                        columnSpacing: 6
                                        rowSpacing: 6
                                        Repeater {
                                            model: [
                                                { t: "上一曲", v: ADT.PreviousSong },
                                                { t: "播放", v: ADT.PlayAndPause },
                                                { t: "下一曲", v: ADT.NextSong },
                                                { t: "停止", v: ADT.StopPlay },
                                                { t: "音量-", v: ADT.VolumeReduce },
                                                { t: "音量+", v: ADT.VolumeAdd }
                                            ]
                                            ActionButton { label: modelData.t; dense: true; Layout.fillWidth: true; enabled: !!page.device; onPressed: DeviceControl.control(ADT.Music, modelData.v) }
                                        }
                                    }
                                }

                                ColumnLayout {
                                    Layout.fillWidth: true
                                    spacing: 7
                                    Header { title: "电池伪装"; subtitle: "调试场景" }
                                    RowLayout {
                                        Layout.fillWidth: true
                                        FluTextBox { id: batteryLevelInput; text: "100"; placeholderText: "电量"; Layout.fillWidth: true }
                                        ActionButton { label: "设置"; dense: true; Layout.preferredWidth: 70; enabled: !!page.device; onPressed: BatteryDisguise.setBatteryLevel(parseInt(batteryLevelInput.text) || 100) }
                                    }
                                    GridLayout {
                                        Layout.fillWidth: true
                                        columns: 2
                                        columnSpacing: 6
                                        rowSpacing: 6
                                        ActionButton { label: "停止充电"; dense: true; Layout.fillWidth: true; enabled: !!page.device; onPressed: BatteryDisguise.stopCharge() }
                                        ActionButton { label: "USB不充电"; dense: true; Layout.fillWidth: true; enabled: !!page.device; onPressed: BatteryDisguise.connectButNoCharge() }
                                        ActionButton { label: "恢复充电"; dense: true; Layout.fillWidth: true; enabled: !!page.device; onPressed: BatteryDisguise.restoreCharge() }
                                        ActionButton { label: "全部重置"; dense: true; Layout.fillWidth: true; enabled: !!page.device; accent: "#d83b01"; onPressed: BatteryDisguise.restoreAll() }
                                    }
                                }

                                ColumnLayout {
                                    Layout.fillWidth: true
                                    spacing: 7
                                    Header { title: "分辨率 / DPI"; subtitle: page.device ? page.device.resolution + " / " + page.device.dpi : "" }
                                    GridLayout {
                                        Layout.fillWidth: true
                                        columns: 3
                                        columnSpacing: 6
                                        rowSpacing: 6
                                        FluTextBox { id: resW; text: page.resolutionPart(0, "1080"); placeholderText: "宽"; Layout.fillWidth: true }
                                        FluTextBox { id: resH; text: page.resolutionPart(1, "1920"); placeholderText: "高"; Layout.fillWidth: true }
                                        FluTextBox { id: resDpi; text: page.device ? (page.device.dpi || "420") : "420"; placeholderText: "DPI"; Layout.fillWidth: true }
                                        ActionButton { label: "恢复"; dense: true; Layout.fillWidth: true; enabled: !!page.device; onPressed: ResolutionControl.restore() }
                                        ActionButton {
                                            label: "应用"
                                            dense: true
                                            Layout.fillWidth: true
                                            Layout.columnSpan: 2
                                            enabled: !!page.device
                                            onPressed: {
                                                var w = parseInt(resW.text)
                                                var h = parseInt(resH.text)
                                                var d = parseInt(resDpi.text)
                                                if (w && h && d) ResolutionControl.set(w, h, d)
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }

                    FluPivotItem {
                        title: "快捷入口"
                        contentItem: Component {
                            GridLayout {
                                anchors { fill: parent; margins: 10 }
                                columns: 4
                                columnSpacing: 8
                                rowSpacing: 8

                                ActionCard { title: "文件推送"; subtitle: "选择文件并传到 /sdcard"; icon: FluentIcons.Document; onPressed: page.openTool(2) }
                                ActionCard { title: "键盘输入"; subtitle: "发送按键与文本"; icon: FluentIcons.KeyboardClassic; onPressed: page.openTool(2) }
                                ActionCard { title: "应用管理"; subtitle: "安装、启动、卸载"; icon: FluentIcons.AllApps; onPressed: page.openTool(3) }
                                ActionCard { title: "刷机维护"; subtitle: "Fastboot 与镜像"; icon: FluentIcons.DeveloperTools; accent: "#ca8a04"; onPressed: page.openTool(4) }
                                ActionCard { title: "实时日志"; subtitle: "ADB logcat"; icon: FluentIcons.ReportDocument; onPressed: page.openTool(5) }
                                ActionCard { title: "投屏质量"; subtitle: "FPS / 码率预设"; icon: FluentIcons.PlayerSettings; onPressed: fpsPopup.open() }
                                ActionCard { title: "截图"; subtitle: "保存当前屏幕"; icon: FluentIcons.Camera; enabled: !!page.device; onPressed: ImageDetailTools.shotScreen("") }
                                ActionCard { title: "重启设备"; subtitle: "系统重启"; icon: FluentIcons.Refresh; accent: "#d83b01"; enabled: !!page.device; onPressed: DeviceControl.control(ADT.Key, ADT.Reboot) }
                            }
                        }
                    }

                    FluPivotItem {
                        title: "文件与输入"
                        contentItem: Component {
                            GridLayout {
                                anchors { fill: parent; margins: 10 }
                                columns: 2
                                columnSpacing: 14

                                ColumnLayout {
                                    Layout.fillWidth: true
                                    spacing: 7
                                    Header { title: "文件传输"; subtitle: "推送到设备" }
                                    RowLayout {
                                        Layout.fillWidth: true
                                        FluTextBox { text: page.transferLocalPath; placeholderText: "本地文件"; Layout.fillWidth: true; onTextChanged: page.transferLocalPath = text }
                                        ActionButton { label: "浏览"; dense: true; Layout.preferredWidth: 70; onPressed: fileDialog.open() }
                                    }
                                    RowLayout {
                                        Layout.fillWidth: true
                                        FluTextBox { id: remotePath; text: "/sdcard/"; placeholderText: "设备路径"; Layout.fillWidth: true }
                                        ActionButton { label: "推送"; dense: true; Layout.preferredWidth: 70; enabled: !!page.device; onPressed: FileTransfer.transmission(page.transferLocalPath, remotePath.text) }
                                    }
                                }

                                ColumnLayout {
                                    Layout.fillWidth: true
                                    spacing: 7
                                    Header { title: "输入 / Activity"; subtitle: "轻量调试" }
                                    Rectangle {
                                        Layout.fillWidth: true
                                        Layout.preferredHeight: 34
                                        radius: 7
                                        color: keyboardArea.activeFocus ? Qt.rgba(0.06, 0.48, 0.42, 0.14) : (FluTheme.dark ? Qt.rgba(1,1,1,0.055) : Qt.rgba(0,0,0,0.035))
                                        border.color: keyboardArea.activeFocus ? "#0f7b6c" : FluTheme.dividerColor
                                        FluText { anchors.centerIn: parent; text: keyboardArea.activeFocus ? "键盘输入监听中" : "点击发送键盘输入"; color: FluTheme.fontSecondaryColor }
                                        MouseArea {
                                            id: keyboardArea
                                            anchors.fill: parent
                                            focus: true
                                            activeFocusOnTab: true
                                            Keys.onPressed: function(e) { InputText.pushKey(e.key); e.accepted = true }
                                        }
                                    }
                                    RowLayout {
                                        Layout.fillWidth: true
                                        FluTextBox { id: activityName; text: "com.tencent.mm/.ui.LauncherUI"; placeholderText: "Activity"; Layout.fillWidth: true }
                                        ActionButton { label: "启动"; dense: true; Layout.preferredWidth: 70; enabled: !!page.device; onPressed: StartActivity.start(activityName.text, "") }
                                    }
                                }
                            }
                        }
                    }

                    FluPivotItem {
                        title: "应用"
                        contentItem: Component {
                            RowLayout {
                                anchors { fill: parent; margins: 8 }
                                spacing: 8

                                ColumnLayout {
                                    Layout.preferredWidth: 420
                                    Layout.minimumWidth: 420
                                    Layout.maximumWidth: 420
                                    Layout.fillHeight: true
                                    spacing: 6
                                    RowLayout {
                                        Layout.fillWidth: true
                                        spacing: 6
                                        FluComboBox {
                                            model: ["第三方", "系统", "全部"]
                                            currentIndex: 0
                                            Layout.preferredWidth: 110
                                            onCurrentIndexChanged: {
                                                page.selectApp("", "", "", "")
                                                AppDetailControl.softListType = currentIndex
                                            }
                                        }
                                        FluText { text: homeAppList.count + " 个应用"; font: FluTextStyle.Caption; color: FluTheme.fontSecondaryColor; Layout.fillWidth: true }
                                        ActionButton { label: "安装 APK"; icon: FluentIcons.Add; dense: true; Layout.preferredWidth: 104; onPressed: apkDialog.open() }
                                    }
                                    ListView {
                                        id: homeAppList
                                        Layout.fillWidth: true
                                        Layout.fillHeight: true
                                        clip: true
                                        model: SoftListModel
                                        spacing: 3
                                        delegate: Rectangle {
                                            width: ListView.view.width
                                            height: 44
                                            radius: 7
                                            color: page.selectedPackage === model.packageName
                                                   ? Qt.rgba(0.06, 0.48, 0.42, FluTheme.dark ? 0.28 : 0.14)
                                                   : (mouse.containsMouse ? (FluTheme.dark ? Qt.rgba(1,1,1,0.055) : Qt.rgba(0,0,0,0.035)) : "transparent")
                                            RowLayout {
                                                anchors { fill: parent; leftMargin: 8; rightMargin: 8; topMargin: 4; bottomMargin: 4 }
                                                spacing: 9
                                                AppIconBox {
                                                    Layout.preferredWidth: 32
                                                    Layout.preferredHeight: 32
                                                    source: model.icon || ""
                                                    title: model.appName || model.packageName || ""
                                                    accent: model.isSystemApp ? "#64748b" : "#0f7b6c"
                                                }
                                                ColumnLayout {
                                                    Layout.fillWidth: true
                                                    Layout.minimumWidth: 0
                                                    spacing: 0
                                                    FluText { text: model.appName || model.packageName || "?"; font: FluTextStyle.Body; Layout.fillWidth: true; Layout.minimumWidth: 0; elide: Text.ElideRight }
                                                    FluText { text: model.packageName || ""; font: FluTextStyle.Caption; color: FluTheme.fontSecondaryColor; Layout.fillWidth: true; Layout.minimumWidth: 0; elide: Text.ElideMiddle }
                                                }
                                                FluText {
                                                    text: model.versionName || ""
                                                    font: FluTextStyle.Caption
                                                    color: FluTheme.fontSecondaryColor
                                                    Layout.preferredWidth: 52
                                                    elide: Text.ElideRight
                                                    horizontalAlignment: Text.AlignRight
                                                }
                                            }
                                            Timer {
                                                interval: 100 + Math.min(index, 8) * 60
                                                running: !model.icon && (model.packageName || "").length > 0
                                                repeat: false
                                                onTriggered: AppDetailControl.requestLoadIcon(model.packageName || "")
                                            }
                                            MouseArea {
                                                id: mouse
                                                anchors.fill: parent
                                                hoverEnabled: true
                                                onClicked: {
                                                    homeAppList.currentIndex = index
                                                    page.selectApp(model.packageName, model.appName, model.versionName, model.icon)
                                                }
                                            }
                                        }
                                    }
                                }

                                ColumnLayout {
                                    Layout.fillWidth: true
                                    Layout.minimumWidth: 0
                                    Layout.fillHeight: true
                                    spacing: 7

                                    RowLayout {
                                        Layout.fillWidth: true
                                        Layout.minimumWidth: 0
                                        spacing: 10
                                        AppIconBox {
                                            Layout.preferredWidth: 44
                                            Layout.preferredHeight: 44
                                            source: page.selectedAppIcon
                                            title: page.selectedAppName || page.selectedPackage
                                            accent: "#0f7b6c"
                                        }
                                        ColumnLayout {
                                            Layout.fillWidth: true
                                            Layout.minimumWidth: 0
                                            spacing: 0
                                            FluText {
                                                text: page.selectedAppName || page.selectedPackage || "选择应用"
                                                font: FluTextStyle.BodyStrong
                                                Layout.fillWidth: true
                                                Layout.minimumWidth: 0
                                                elide: Text.ElideRight
                                            }
                                            FluText {
                                                text: page.selectedPackage.length > 0 ? page.selectedPackage : "从左侧列表选择应用后管理"
                                                font: FluTextStyle.Caption
                                                color: FluTheme.fontSecondaryColor
                                                Layout.fillWidth: true
                                                Layout.minimumWidth: 0
                                                elide: Text.ElideMiddle
                                            }
                                        }
                                    }

                                    GridLayout {
                                        Layout.fillWidth: true
                                        columns: 6
                                        columnSpacing: 6
                                        rowSpacing: 6
                                        ActionButton { label: "启动"; dense: true; Layout.fillWidth: true; enabled: !!page.selectedPackage; onPressed: AppDetailControl.startApp(page.selectedPackage) }
                                        ActionButton { label: "停止"; dense: true; Layout.fillWidth: true; enabled: !!page.selectedPackage; onPressed: AppDetailControl.stopApp(page.selectedPackage) }
                                        ActionButton { label: "提取"; dense: true; Layout.fillWidth: true; enabled: !!page.selectedPackage; onPressed: extractDialog.open() }
                                        ActionButton { label: "冻结"; dense: true; Layout.fillWidth: true; enabled: !!page.selectedPackage; onPressed: AppDetailControl.freezeApp(page.selectedPackage) }
                                        ActionButton { label: "清数据"; dense: true; Layout.fillWidth: true; enabled: !!page.selectedPackage; accent: "#ca8a04"; onPressed: AppDetailControl.clearData(page.selectedPackage) }
                                        ActionButton { label: "卸载"; dense: true; Layout.fillWidth: true; enabled: !!page.selectedPackage; accent: "#d83b01"; onPressed: AppDetailControl.uninstallApp(page.selectedPackage) }
                                    }

                                    GridLayout {
                                        Layout.fillWidth: true
                                        columns: 2
                                        columnSpacing: 6
                                        rowSpacing: 6
                                        CompactTile { label: "版本"; value: page.selectedAppVersion || (AppDetailControl.versionCode > 0 ? String(AppDetailControl.versionCode) : "--"); accent: "#2563eb" }
                                        CompactTile { label: "版本码"; value: AppDetailControl.versionCode > 0 ? String(AppDetailControl.versionCode) : "--"; accent: "#2563eb" }
                                        CompactTile { label: "安装"; value: AppDetailControl.installDate || "--"; accent: "#64748b" }
                                        CompactTile { label: "SDK"; value: (AppDetailControl.minSdk || "--") + " -> " + (AppDetailControl.targetSdk || "--"); accent: "#7c3aed" }
                                    }
                                }
                            }
                        }
                    }

                    FluPivotItem {
                        title: "刷机"
                        contentItem: Component {
                            GridLayout {
                                anchors { fill: parent; margins: 10 }
                                columns: 3
                                columnSpacing: 14

                                ColumnLayout {
                                    Layout.fillWidth: true
                                    spacing: 7
                                    Header { title: "Fastboot"; subtitle: FastBootDeviceManager.currentDeviceCode || "无设备" }
                                    ActionButton { label: "刷新设备"; dense: true; Layout.fillWidth: true; onPressed: FastBootDeviceManager.updateDevices() }
                                    ActionButton { label: "重启到系统"; dense: true; Layout.fillWidth: true; enabled: !!FastBootDeviceManager.currentDeviceCode; onPressed: FastBootDeviceManager.rebootToSystem("") }
                                    ActionButton { label: "关机"; dense: true; Layout.fillWidth: true; enabled: !!FastBootDeviceManager.currentDeviceCode; accent: "#d83b01"; onPressed: FastBootDeviceManager.powerOff("") }
                                }

                                ColumnLayout {
                                    Layout.fillWidth: true
                                    spacing: 7
                                    Header { title: "临时启动 / 刷入"; subtitle: "镜像文件" }
                                    RowLayout { Layout.fillWidth: true; FluTextBox { text: page.fastbootImagePath; placeholderText: "boot.img / image.img"; Layout.fillWidth: true; onTextChanged: page.fastbootImagePath = text } ActionButton { label: "..."; dense: true; Layout.preferredWidth: 44; onPressed: imageDialog.open() } }
                                    RowLayout { Layout.fillWidth: true; FluTextBox { id: partitionName; text: "boot"; placeholderText: "分区"; Layout.fillWidth: true } }
                                    RowLayout {
                                        Layout.fillWidth: true
                                        ActionButton { label: "临时启动"; dense: true; Layout.fillWidth: true; enabled: !!FastBootDeviceManager.currentDeviceCode; onPressed: FlashTools.startBoot("", page.fastbootImagePath) }
                                        ActionButton { label: "刷入"; dense: true; Layout.fillWidth: true; enabled: !!FastBootDeviceManager.currentDeviceCode; accent: "#ca8a04"; onPressed: FlashTools.flash("", partitionName.text, page.fastbootImagePath) }
                                    }
                                }

                                ColumnLayout {
                                    Layout.fillWidth: true
                                    spacing: 7
                                    Header { title: "维护"; subtitle: "危险操作" }
                                    RowLayout { Layout.fillWidth: true; FluTextBox { id: erasePart; text: "cache"; placeholderText: "分区"; Layout.fillWidth: true } ActionButton { label: "擦除"; dense: true; Layout.preferredWidth: 70; enabled: !!FastBootDeviceManager.currentDeviceCode; accent: "#d83b01"; onPressed: FlashTools.clear("", erasePart.text) } }
                                    RowLayout { Layout.fillWidth: true; FluTextBox { text: page.flashZipPath; placeholderText: "压缩包"; Layout.fillWidth: true; onTextChanged: page.flashZipPath = text } ActionButton { label: "..."; dense: true; Layout.preferredWidth: 44; onPressed: zipDialog.open() } }
                                }
                            }
                        }
                    }

                    FluPivotItem {
                        title: "日志"
                        contentItem: Component {
                            ColumnLayout {
                                anchors { fill: parent; margins: 10 }
                                spacing: 6
                                Header { title: "ADB 日志"; subtitle: ADBLog.rowCount() + " 条" }
                                ListView {
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true
                                    clip: true
                                    model: ADBLog
                                    delegate: Rectangle {
                                        width: ListView.view.width
                                        height: Math.max(22, logText.implicitHeight + 5)
                                        color: index % 2 ? "transparent" : (FluTheme.dark ? Qt.rgba(1,1,1,0.03) : Qt.rgba(0,0,0,0.025))
                                        FluText {
                                            id: logText
                                            anchors { left: parent.left; right: parent.right; verticalCenter: parent.verticalCenter; margins: 6 }
                                            text: model.display || ""
                                            font.pixelSize: 12
                                            font.family: "Menlo"
                                            wrapMode: Text.WrapAnywhere
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    FileDialog { id: fileDialog; title: "选择文件"; fileMode: FileDialog.OpenFile; onAccepted: page.transferLocalPath = page.localPath(currentFile) }
    FileDialog { id: apkDialog; title: "选择 APK"; nameFilters: ["APK files (*.apk)"]; onAccepted: AppDetailControl.installApp(page.localPath(currentFile)) }
    FileDialog { id: imageDialog; title: "选择镜像"; onAccepted: page.fastbootImagePath = page.localPath(currentFile) }
    FileDialog { id: zipDialog; title: "选择压缩包"; onAccepted: page.flashZipPath = page.localPath(currentFile) }
    FolderDialog { id: extractDialog; title: "选择 APK 保存目录"; onAccepted: AppDetailControl.extractApp(page.selectedPackage, page.localPath(selectedFolder)) }
}
