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
import "qrc:/qml2/pages/home"

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
    property int workbenchIndex: 0
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

    function mirrorFrameRatio() {
        try {
            if (mirrorView.image && mirrorView.image.width > 0 && mirrorView.image.height > 0)
                return mirrorView.image.width / mirrorView.image.height
        } catch (e) {
        }
        if (Resource.frameSize && Resource.frameSize.width > 0 && Resource.frameSize.height > 0)
            return Resource.frameSize.width / Resource.frameSize.height
        return Resource.orientation === 1 ? 16 / 9 : 9 / 16
    }

    function openTool(index) {
        page.workbenchIndex = index
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
        id: mirrorPopup
        width: Math.min((parent ? parent.width : 1180) - 56, 980)
        height: Math.min((parent ? parent.height : 760) - 56, 680)
        x: parent ? (parent.width - width) / 2 : 28
        y: parent ? (parent.height - height) / 2 : 28
        padding: 12
        modal: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
        background: Rectangle {
            radius: 10
            color: "#050608"
            border.width: 1
            border.color: FluTheme.dark ? Qt.rgba(1, 1, 1, 0.16) : Qt.rgba(0, 0, 0, 0.28)
        }
        contentItem: Loader {
            id: mirrorPopupLoader
            active: mirrorPopup.visible
            onLoaded: item.frameRatio = Qt.binding(function() { return page.mirrorFrameRatio() })
            sourceComponent: Component {
                Rectangle {
                    property real frameRatio: 9 / 16
                    color: "#050608"
                    radius: 8
                    clip: true

                    Item {
                        id: popupMirrorSurface
                        width: parent.width / parent.height > parent.frameRatio ? parent.height * parent.frameRatio : parent.width
                        height: parent.width / parent.height > parent.frameRatio ? parent.height : parent.width / parent.frameRatio
                        anchors.centerIn: parent

                        ImageFrameItem {
                            anchors.fill: parent
                            imageFrame: mirrorView.image
                            hasAlphaChannel: false
                        }

                        MirrorScene {
                            id: popupMirrorView
                            anchors.fill: parent
                        }
                    }

                    Rectangle {
                        anchors { right: parent.right; top: parent.top; margins: 10 }
                        width: 34
                        height: 30
                        radius: 7
                        color: closeMouse.containsMouse ? Qt.rgba(1, 1, 1, 0.18) : Qt.rgba(1, 1, 1, 0.10)
                        FluIcon { anchors.centerIn: parent; iconSource: FluentIcons.ChromeClose; iconSize: 12; iconColor: "#e5e7eb" }
                        MouseArea {
                            id: closeMouse
                            anchors.fill: parent
                            hoverEnabled: true
                            onClicked: mirrorPopup.close()
                        }
                    }
                }
            }
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
                            ActionButton { label: "放大"; icon: FluentIcons.FullScreen; dense: true; Layout.preferredWidth: 72; enabled: !!page.device; onPressed: mirrorPopup.open() }
                        }

                        RowLayout {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 288
                            spacing: 8
                            layoutDirection: Qt.LeftToRight

                            Rectangle {
                                Layout.fillWidth: true
                                Layout.minimumWidth: 240
                                Layout.preferredWidth: Math.max(260, parent.width - 140)
                                Layout.fillHeight: true
                                radius: 10
                                color: "#050608"
                                border.width: 1
                                border.color: FluTheme.dark ? Qt.rgba(1, 1, 1, 0.12) : Qt.rgba(0, 0, 0, 0.18)
                                clip: true

                                Item {
                                    id: mirrorSurface
                                    property real frameRatio: page.mirrorFrameRatio()
                                    width: parent.width / parent.height > frameRatio ? parent.height * frameRatio : parent.width
                                    height: parent.width / parent.height > frameRatio ? parent.height : parent.width / frameRatio
                                    anchors.centerIn: parent

                                    ImageFrameItem {
                                        anchors.fill: parent
                                        imageFrame: mirrorView.image
                                        hasAlphaChannel: false
                                    }

                                    MirrorScene {
                                        id: mirrorView
                                        anchors.fill: parent
                                        z: 1
                                    }
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

                            ColumnLayout {
                                Layout.preferredWidth: 132
                                Layout.minimumWidth: 132
                                Layout.maximumWidth: 132
                                Layout.fillHeight: true
                                spacing: 6

                                GridLayout {
                                    Layout.fillWidth: true
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

                                Rectangle {
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true
                                    radius: 7
                                    color: FluTheme.dark ? Qt.rgba(1,1,1,0.045) : Qt.rgba(0,0,0,0.028)
                                    border.width: 1
                                    border.color: FluTheme.dividerColor

                                    ColumnLayout {
                                        anchors { fill: parent; margins: 6 }
                                        spacing: 4
                                        GridLayout {
                                            Layout.fillWidth: true
                                            columns: 1
                                            rowSpacing: 4
                                            ActionButton {
                                                label: "省电"
                                                icon: FluentIcons.QuietHours
                                                dense: true
                                                Layout.fillWidth: true
                                                enabled: !!page.device
                                                onPressed: {
                                                    ScrcpyConfig.maxFps = 10
                                                    ScrcpyConfig.kBitRate = 2500
                                                    page.restartMirror()
                                                }
                                            }
                                            ActionButton {
                                                label: "均衡"
                                                icon: FluentIcons.SpeedHigh
                                                dense: true
                                                Layout.fillWidth: true
                                                enabled: !!page.device
                                                onPressed: {
                                                    ScrcpyConfig.maxFps = 24
                                                    ScrcpyConfig.kBitRate = 6000
                                                    page.restartMirror()
                                                }
                                            }
                                            ActionButton {
                                                label: "清晰"
                                                icon: FluentIcons.Video
                                                dense: true
                                                Layout.fillWidth: true
                                                enabled: !!page.device
                                                onPressed: {
                                                    ScrcpyConfig.maxFps = 30
                                                    ScrcpyConfig.kBitRate = 9000
                                                    page.restartMirror()
                                                }
                                            }
                                        }
                                    }
                                }
                            }
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

                ColumnLayout {
                    anchors { fill: parent; margins: 10 }
                    spacing: 6

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 6
                        WorkbenchTab { label: "控制"; icon: FluentIcons.Permissions; tabIndex: 0; currentIndex: page.workbenchIndex; onActivated: function(index) { page.workbenchIndex = index } }
                        WorkbenchTab { label: "快捷"; icon: FluentIcons.FavoriteStar; tabIndex: 1; currentIndex: page.workbenchIndex; accent: "#2563eb"; onActivated: function(index) { page.workbenchIndex = index } }
                        WorkbenchTab { label: "文件"; icon: FluentIcons.Document; tabIndex: 2; currentIndex: page.workbenchIndex; accent: "#2563eb"; onActivated: function(index) { page.workbenchIndex = index } }
                        WorkbenchTab { label: "应用"; icon: FluentIcons.AllApps; tabIndex: 3; currentIndex: page.workbenchIndex; accent: "#0f7b6c"; onActivated: function(index) { page.workbenchIndex = index } }
                        WorkbenchTab { label: "刷机"; icon: FluentIcons.DeveloperTools; tabIndex: 4; currentIndex: page.workbenchIndex; accent: "#ca8a04"; onActivated: function(index) { page.workbenchIndex = index } }
                        WorkbenchTab { label: "日志"; icon: FluentIcons.ReportDocument; tabIndex: 5; currentIndex: page.workbenchIndex; accent: "#64748b"; onActivated: function(index) { page.workbenchIndex = index } }
                    }

                    StackLayout {
                        id: workbench
                        currentIndex: page.workbenchIndex
                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        Item {
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

                        Item {
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
                                ActionCard { title: "截图"; subtitle: "保存当前屏幕"; icon: FluentIcons.Camera; enabled: !!page.device; onPressed: ImageDetailTools.shotScreen("") }
                                ActionCard { title: "重启设备"; subtitle: "系统重启"; icon: FluentIcons.Refresh; accent: "#d83b01"; enabled: !!page.device; onPressed: DeviceControl.control(ADT.Key, ADT.Reboot) }
                            }
                        }

                        Item {
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

                        Item {
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

                        Item {
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

                        Item {
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
