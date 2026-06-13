import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import FluentUI
import ConnectManager 1.0
import SystemInfo 1.0
import Resource 1.0
import "qrc:/qml2/pages/home"
import "qrc:/qml2/pages/performance"

FluContentPage {
    id: page
    title: ""

    property var device: ConnectManager.cutADBDevice
    property bool recording: true
    property int sampleInterval: 1000
    property int sampleCount: 0
    property real ramPct: SystemInfo.ramTotal > 0 ? Math.round(SystemInfo.ramUsage / SystemInfo.ramTotal * 100) : 0
    property real storagePct: SystemInfo.storageTotal > 0 ? Math.round(SystemInfo.storageUsed / SystemInfo.storageTotal * 100) : 0
    property real fpsPct: Math.min(100, Math.max(0, SystemInfo.fps / 60 * 100))
    property real tempPct: Math.min(100, Math.max(0, SystemInfo.cpuTemp))
    property string selectedPackage: SystemInfo.foregroundPackage || (device ? device.currentPackage : "")

    Component.onCompleted: {
        SystemInfo.startPolling()
        sampleTimer.start()
    }

    Component.onDestruction: {
        sampleTimer.stop()
    }

    function textOrDash(value) {
        return value === undefined || value === null || String(value).length === 0 ? "--" : String(value)
    }

    function formatGb(value) {
        return value > 0 ? value.toFixed(1) + " GB" : "--"
    }

    function clearSamples() {
        sampleCount = 0
        perfChart.clear()
    }

    Timer {
        id: sampleTimer
        interval: page.sampleInterval
        repeat: true
        running: true
        onTriggered: {
            if (!page.recording)
                return
            page.sampleCount += 1
            perfChart.appendSample(page.fpsPct, SystemInfo.cpuUsage, SystemInfo.gpuUsage, page.ramPct, page.tempPct)
        }
    }

    ScrollView {
        anchors.fill: parent
        contentWidth: availableWidth
        ScrollBar.vertical.policy: ScrollBar.AsNeeded

        ColumnLayout {
            width: parent.width
            spacing: 8

            Panel {
                Layout.fillWidth: true
                Layout.preferredHeight: 84
                Layout.leftMargin: 12
                Layout.rightMargin: 12
                Layout.topMargin: 8

                RowLayout {
                    anchors { fill: parent; margins: 12 }
                    spacing: 12

                    Rectangle {
                        Layout.preferredWidth: 38
                        Layout.preferredHeight: 38
                        radius: 8
                        color: Qt.rgba(0.06, 0.48, 0.42, FluTheme.dark ? 0.24 : 0.12)
                        FluIcon { anchors.centerIn: parent; iconSource: FluentIcons.SpeedHigh; iconSize: 20; iconColor: "#0f7b6c" }
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 2
                        FluText {
                            text: "性能监控"
                            font.pixelSize: 18
                            font.bold: true
                            color: FluTheme.fontPrimaryColor
                        }
                        FluText {
                            text: (device ? (device.brand + " " + device.model + " / " + (device.serialNumber || device.deviceCode || "-")) : "未连接设备")
                                  + "    前台 " + page.textOrDash(page.selectedPackage)
                            font: FluTextStyle.Caption
                            color: FluTheme.fontSecondaryColor
                            elide: Text.ElideRight
                            Layout.fillWidth: true
                        }
                    }

                    Rectangle {
                        Layout.preferredWidth: 96
                        Layout.preferredHeight: 32
                        radius: 7
                        color: page.recording ? Qt.rgba(0.06, 0.48, 0.42, FluTheme.dark ? 0.22 : 0.12)
                                              : Qt.rgba(0.50, 0.50, 0.50, 0.10)
                        border.width: 1
                        border.color: page.recording ? "#0f7b6c" : FluTheme.dividerColor
                        RowLayout {
                            anchors.centerIn: parent
                            spacing: 5
                            Rectangle { Layout.preferredWidth: 7; Layout.preferredHeight: 7; radius: 4; color: page.recording ? "#0f7b6c" : "#64748b" }
                            FluText { text: page.recording ? "采集中" : "已暂停"; font: FluTextStyle.Caption }
                        }
                    }

                    FluButton {
                        text: page.recording ? "暂停" : "开始"
                        Layout.preferredWidth: 74
                        onClicked: page.recording = !page.recording
                    }
                    FluButton {
                        text: "清空"
                        Layout.preferredWidth: 74
                        onClicked: page.clearSamples()
                    }
                }
            }

            GridLayout {
                Layout.fillWidth: true
                Layout.leftMargin: 12
                Layout.rightMargin: 12
                columns: 6
                columnSpacing: 8
                rowSpacing: 8

                PerfMetricCard { label: "FPS"; value: SystemInfo.fps > 0 ? String(SystemInfo.fps) : "--"; unit: "fps"; detail: "目标 60 / 样本 " + page.sampleCount; level: page.fpsPct; accent: "#d83b01" }
                PerfMetricCard { label: "CPU"; value: Math.round(SystemInfo.cpuUsage) + "%"; detail: SystemInfo.cpuCoreCount > 0 ? SystemInfo.cpuCoreCount + " 核 / " + SystemInfo.cpuMaxFrequency + " MHz" : "等待 CPU 数据"; level: SystemInfo.cpuUsage; accent: "#0f7b6c" }
                PerfMetricCard { label: "GPU"; value: Math.round(SystemInfo.gpuUsage) + "%"; detail: page.textOrDash(SystemInfo.gpuName) + " / " + SystemInfo.gpuCurrentFrequency + " MHz"; level: SystemInfo.gpuUsage; accent: "#2563eb" }
                PerfMetricCard { label: "RAM"; value: page.ramPct + "%"; detail: page.formatGb(SystemInfo.ramUsage) + " / " + page.formatGb(SystemInfo.ramTotal); level: page.ramPct; accent: "#7c3aed" }
                PerfMetricCard { label: "温度"; value: SystemInfo.cpuTemp > 0 ? SystemInfo.cpuTemp.toFixed(1) : "--"; unit: "C"; detail: "GPU " + (SystemInfo.gpuTemperature >= 0 ? SystemInfo.gpuTemperature + "C" : "--"); level: page.tempPct; accent: "#ca8a04" }
                PerfMetricCard { label: "电池"; value: device ? device.batteryLevel + "%" : "--"; detail: device ? (device.batteryVoltage / 1000.0).toFixed(2) + "V / " + device.batteryCurrent + " mA" : page.textOrDash(SystemInfo.batteryTechnology); level: device ? device.batteryLevel : 0; accent: "#64748b" }
            }

            RowLayout {
                Layout.fillWidth: true
                Layout.leftMargin: 12
                Layout.rightMargin: 12
                Layout.preferredHeight: 348
                spacing: 10

                Panel {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    ColumnLayout {
                        anchors { fill: parent; margins: 12 }
                        spacing: 8

                        RowLayout {
                            Layout.fillWidth: true
                            Header { title: "实时曲线"; subtitle: "FPS / CPU / GPU / RAM / TEMP"; Layout.fillWidth: true }
                            LegendPill { label: "FPS"; value: ""; accent: "#d83b01" }
                            LegendPill { label: "CPU"; value: ""; accent: "#0f7b6c" }
                            LegendPill { label: "GPU"; value: ""; accent: "#2563eb" }
                            LegendPill { label: "RAM"; value: ""; accent: "#7c3aed" }
                            LegendPill { label: "TEMP"; value: ""; accent: "#ca8a04" }
                        }

                        PerfChart {
                            id: perfChart
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                        }
                    }
                }

                Panel {
                    Layout.preferredWidth: 326
                    Layout.fillHeight: true

                    ColumnLayout {
                        anchors { fill: parent; margins: 12 }
                        spacing: 8

                        Header { title: "前台应用"; subtitle: SystemInfo.foregroundPid > 0 ? "PID " + SystemInfo.foregroundPid : "等待数据" }

                        RowLayout {
                            Layout.fillWidth: true
                            spacing: 10
                            AppIconBox {
                                Layout.preferredWidth: 48
                                Layout.preferredHeight: 48
                                source: ""
                                title: SystemInfo.foregroundAppName || page.selectedPackage
                                accent: "#0f7b6c"
                            }
                            ColumnLayout {
                                Layout.fillWidth: true
                                spacing: 1
                                FluText { text: page.textOrDash(SystemInfo.foregroundAppName); font: FluTextStyle.BodyStrong; Layout.fillWidth: true; elide: Text.ElideRight }
                                FluText { text: page.textOrDash(page.selectedPackage); font: FluTextStyle.Caption; color: FluTheme.fontSecondaryColor; Layout.fillWidth: true; elide: Text.ElideMiddle }
                                FluText { text: page.textOrDash(SystemInfo.foregroundActivity); font: FluTextStyle.Caption; color: FluTheme.fontSecondaryColor; Layout.fillWidth: true; elide: Text.ElideMiddle }
                            }
                        }

                        GridLayout {
                            Layout.fillWidth: true
                            columns: 2
                            columnSpacing: 6
                            rowSpacing: 6
                            CompactTile { label: "版本"; value: page.textOrDash(SystemInfo.foregroundVersion); accent: "#2563eb" }
                            CompactTile { label: "UID"; value: SystemInfo.foregroundUid > 0 ? String(SystemInfo.foregroundUid) : "--"; accent: "#64748b" }
                            CompactTile { label: "进程"; value: SystemInfo.foregroundPid > 0 ? String(SystemInfo.foregroundPid) : "--"; accent: "#0f7b6c" }
                            CompactTile { label: "内存"; value: SystemInfo.foregroundMemoryMB > 0 ? SystemInfo.foregroundMemoryMB.toFixed(0) + " MB" : "--"; accent: "#7c3aed" }
                            CompactTile { label: "CPU"; value: SystemInfo.foregroundCpuUsage > 0 ? SystemInfo.foregroundCpuUsage.toFixed(1) + "%" : "--"; accent: "#0f7b6c" }
                            CompactTile { label: "刷新"; value: (page.sampleInterval / 1000).toFixed(1) + "s"; accent: "#ca8a04" }
                        }

                        Rectangle {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            radius: 7
                            color: FluTheme.dark ? Qt.rgba(1,1,1,0.040) : Qt.rgba(0,0,0,0.024)
                            border.width: 1
                            border.color: FluTheme.dividerColor

                            ColumnLayout {
                                anchors { fill: parent; margins: 10 }
                                spacing: 6
                                FluText { text: "采样说明"; font: FluTextStyle.BodyStrong }
                                FluText {
                                    text: "当前版本复用 AndroidToolsServer 已有接口。FPS、进程 CPU、应用内存字段已经预留，后续补 /sample 和 framestats 后可直接接入。"
                                    font: FluTextStyle.Caption
                                    color: FluTheme.fontSecondaryColor
                                    wrapMode: Text.WordWrap
                                    Layout.fillWidth: true
                                }
                            }
                        }
                    }
                }
            }

            RowLayout {
                Layout.fillWidth: true
                Layout.leftMargin: 12
                Layout.rightMargin: 12
                Layout.bottomMargin: 12
                Layout.preferredHeight: 124
                spacing: 10

                Panel {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    ColumnLayout {
                        anchors { fill: parent; margins: 12 }
                        spacing: 8
                        Header { title: "硬件概览"; subtitle: page.textOrDash(SystemInfo.cpuModel) }
                        GridLayout {
                            Layout.fillWidth: true
                            columns: 4
                            columnSpacing: 6
                            rowSpacing: 6
                            CompactTile { label: "CPU"; value: page.textOrDash(SystemInfo.cpuArchitecture); accent: "#0f7b6c" }
                            CompactTile { label: "频率"; value: SystemInfo.cpuMinFrequency + "-" + SystemInfo.cpuMaxFrequency + " MHz"; accent: "#0f7b6c" }
                            CompactTile { label: "GPU"; value: page.textOrDash(SystemInfo.gpuVendor); accent: "#2563eb" }
                            CompactTile { label: "存储"; value: SystemInfo.storageTotal > 0 ? SystemInfo.storageUsed.toFixed(0) + "/" + SystemInfo.storageTotal.toFixed(0) + " GB" : "--"; accent: "#2563eb" }
                            CompactTile { label: "可用内存"; value: page.formatGb(SystemInfo.memAvailable); accent: "#7c3aed" }
                            CompactTile { label: "低内存"; value: SystemInfo.lowMemory ? "是" : "否"; accent: SystemInfo.lowMemory ? "#d83b01" : "#64748b" }
                            CompactTile { label: "电池技术"; value: page.textOrDash(SystemInfo.batteryTechnology); accent: "#ca8a04" }
                            CompactTile { label: "投屏"; value: Resource.mirror > 0 ? "运行中" : "未启动"; accent: Resource.mirror > 0 ? "#0f7b6c" : "#64748b" }
                        }
                    }
                }

                Panel {
                    Layout.preferredWidth: 326
                    Layout.fillHeight: true
                    ColumnLayout {
                        anchors { fill: parent; margins: 12 }
                        spacing: 8
                        Header { title: "下一步能力"; subtitle: "PerfDog 级增强" }
                        CompactTile { label: "聚合"; value: "/sample 一次返回所有指标"; accent: "#0f7b6c" }
                        CompactTile { label: "帧"; value: "framestats / jank / frame time"; accent: "#d83b01" }
                        CompactTile { label: "进程"; value: "process list / per-pid CPU"; accent: "#2563eb" }
                    }
                }
            }
        }
    }
}
