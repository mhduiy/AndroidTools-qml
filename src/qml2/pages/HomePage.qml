import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import FluentUI
import ConnectManager 1.0
import DeviceControl 1.0
import MirrorScene 1.0
import Resource 1.0
import ScrcpyConfig 1.0
import ImageFrameItem 1.0
import ImageDetailTools 1.0
import ADT 1.0
import SystemInfo 1.0
import "qrc:/qml2/components"

FluContentPage {
    id: page; title: "仪表盘"

    property var device: ConnectManager.cutADBDevice
    property bool mirrorActive: Resource.mirror > 0
    property real ramPct: SystemInfo.ramTotal > 0 ? Math.round(SystemInfo.ramUsage / SystemInfo.ramTotal * 100) : 0
    property real battTemp: device ? device.batteryTemperature : 0

    property real phoneW: 260
    property real phoneH: {
        if (!device) return 420
        var parts = device.resolution.split('x')
        var w = parseInt(parts[0]) || 1080, h = parseInt(parts[1]) || 2400
        return Math.min(460, Math.max(330, phoneW * Math.max(h,w) / Math.min(h,w)))
    }

    Timer { id: initTimer; interval: 1000; onTriggered: { if (device) SystemInfo.startPolling() } }
    Connections {
        target: ConnectManager
        function onCutADBDeviceChanged() { if (device) initTimer.start(); else SystemInfo.stopPolling() }
    }
    Component.onCompleted: { if (device) initTimer.start() }
    Component.onDestruction: {
        SystemInfo.stopPolling()
        if (mirrorActive) Resource.qmlRequest("REQUEST_MIRROR_FINISH", "")
    }

    // --- FPS Popup ---
    Popup {
        id: fpsPopup; x: parent.width - width - 20; y: 60; modal: true; padding: 16
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
        background: Rectangle {
            radius: 8; border.color: FluTheme.dividerColor; border.width: 1
            color: FluTheme.dark ? Qt.rgba(0.12,0.12,0.14,0.97) : Qt.rgba(0.98,0.98,0.98,0.97)
        }
        ColumnLayout {
            spacing: 10
            FluText { text: "投屏设置"; font: FluTextStyle.BodyStrong }
            RowLayout {
                FluText { text: "最大帧率"; Layout.preferredWidth: 100 }
                FluTextBox { id: fpsField; Layout.preferredWidth: 80; text: ScrcpyConfig.maxFps }
            }
            RowLayout {
                FluText { text: "码率"; Layout.preferredWidth: 100 }
                FluTextBox { id: brField; Layout.preferredWidth: 100; text: ScrcpyConfig.kBitRate }
            }
            RowLayout { spacing: 6; Layout.topMargin: 4
                FluButton { text: "低"; Layout.fillWidth: true; onClicked: { fpsField.text="2"; brField.text="2000" } }
                FluButton { text: "中"; Layout.fillWidth: true; onClicked: { fpsField.text="15"; brField.text="4000" } }
                FluButton { text: "高"; Layout.fillWidth: true; onClicked: { fpsField.text="30"; brField.text="8000" } }
            }
            FluButton { text: "应用"; Layout.fillWidth: true
                onClicked: { ScrcpyConfig.maxFps = parseInt(fpsField.text) || 30; ScrcpyConfig.kBitRate = parseInt(brField.text) || 8000; restartMirror(); fpsPopup.close() }
            }
        }
    }

    function toggleMirror() { mirrorActive ? stopMirror() : startMirror() }
    function startMirror()  { Resource.qmlRequest("REQUEST_MIRROR_START", "") }
    function stopMirror()   { Resource.qmlRequest("REQUEST_MIRROR_FINISH", "") }
    function restartMirror() { if (mirrorActive) { stopMirror(); startMirror() } }

    Connections {
        target: mirrorView
        function onCppGenerateEvents(request, data) {
            if (request === "MIRROR_START") Resource.mirror = 1
            else if (request === "MIRROR_FINISHED") Resource.mirror = 0
        }
    }

    // --- Monochrome SVG-style icon (Canvas) ---
    component DetailIcon: Canvas {
        id: di
        property string kind: "cpu"
        width: 14; height: 14
        renderStrategy: Canvas.Cooperative; antialiasing: true

        onPaint: {
            var ctx = getContext("2d")
            var c = FluTheme.dark ? "#999" : "#777"
            ctx.clearRect(0, 0, 14, 14)
            ctx.strokeStyle = c; ctx.fillStyle = c; ctx.lineWidth = 1.2; ctx.lineCap = "round"; ctx.lineJoin = "round"

            if (kind === "cpu") {
                ctx.strokeRect(2.5, 2.5, 9, 9)
                ctx.fillRect(5.5, 1, 3, 2.5); ctx.fillRect(5.5, 10.5, 3, 2.5)
                ctx.fillRect(1, 5.5, 2.5, 3); ctx.fillRect(10.5, 5.5, 2.5, 3)
            } else if (kind === "cores") {
                for (var i=0; i<4; i++) { ctx.fillRect(4.5, 2+i*3.2, 6, 2) }
            } else if (kind === "freq") {
                ctx.beginPath(); ctx.moveTo(2,9); ctx.lineTo(5,3); ctx.lineTo(8,9); ctx.lineTo(11,3); ctx.stroke()
            } else if (kind === "memory") {
                ctx.strokeRect(2,3,10,9)
                ctx.beginPath(); ctx.moveTo(5,3); ctx.lineTo(5,12); ctx.moveTo(9,3); ctx.lineTo(9,12); ctx.moveTo(7,3); ctx.lineTo(7,12); ctx.stroke()
                ctx.fillRect(3,1,2,3); ctx.fillRect(9,1,2,3)
            } else if (kind === "storage") {
                ctx.beginPath(); ctx.arc(7,7,5,0,Math.PI*2); ctx.stroke()
                ctx.fillRect(2,6,10,2)
                ctx.fillRect(6.5,2.5,1,3); ctx.fillRect(6.5,8.5,1,3)
            } else if (kind === "resolution") {
                ctx.strokeRect(1.5, 2.5, 11, 8)
                ctx.fillRect(4,1,6,2.5); ctx.fillRect(4,10.5,6,2.5)
            } else if (kind === "dpi") {
                ctx.beginPath(); ctx.arc(7,7,5,0,Math.PI*2); ctx.stroke()
                ctx.beginPath(); ctx.arc(7,7,2,0,Math.PI*2); ctx.fill()
            } else if (kind === "serial") {
                ctx.beginPath(); ctx.arc(7,4.5,3.5,0,Math.PI*2); ctx.stroke()
                ctx.beginPath(); ctx.moveTo(5,7.5); ctx.lineTo(9,7.5); ctx.moveTo(7,7.5); ctx.lineTo(7,12.5); ctx.moveTo(5.5,10.5); ctx.lineTo(8.5,10.5); ctx.stroke()
                ctx.beginPath(); ctx.arc(7,10.5,1,0,Math.PI*2); ctx.fill()
            } else if (kind === "device") {
                ctx.beginPath(); ctx.arc(7,6,1.5,0,Math.PI*2); ctx.fill()
                ctx.strokeRect(3.5,2.5,7,10, 2)
            } else if (kind === "code") {
                ctx.beginPath(); ctx.moveTo(5,3); ctx.lineTo(3,7); ctx.lineTo(5,11); ctx.stroke()
                ctx.beginPath(); ctx.moveTo(9,3); ctx.lineTo(11,7); ctx.lineTo(9,11); ctx.stroke()
            } else if (kind === "system") {
                ctx.beginPath(); ctx.arc(7,7,3,0,Math.PI*2); ctx.stroke()
                ctx.beginPath(); ctx.moveTo(4,4); ctx.lineTo(10,10); ctx.stroke()
                ctx.fillRect(6.5,2,1,2); ctx.fillRect(6.5,10,1,2)
                ctx.fillRect(2,6.5,2,1); ctx.fillRect(10,6.5,2,1)
            } else if (kind === "ip") {
                ctx.beginPath(); ctx.arc(7,7,4.5,0,Math.PI*2); ctx.stroke()
                ctx.beginPath(); ctx.moveTo(2.5,7); ctx.lineTo(11.5,7); ctx.moveTo(7,2.5); ctx.lineTo(7,11.5); ctx.stroke()
                ctx.beginPath(); ctx.ellipse(7,7,8,5); ctx.stroke()
            } else if (kind === "app") {
                ctx.strokeRect(3,3,8,8, 1.5)
                ctx.beginPath(); ctx.moveTo(5,7); ctx.lineTo(7,5); ctx.lineTo(9,7); ctx.stroke()
            }
        }
    }

    // --- Info cell with icon ---
    component InfoCell: RowLayout {
        property string label: ""; property string value: ""; property string icon: ""
        spacing: 6
        DetailIcon { kind: parent.icon ? parent.icon : "cpu"; Layout.alignment: Qt.AlignVCenter }
        FluText { text: label; font: FluTextStyle.Caption; color: FluTheme.fontSecondaryColor; Layout.preferredWidth: 56 }
        FluText { text: value; font: FluTextStyle.Body; elide: Text.ElideRight; Layout.fillWidth: true }
    }

    ScrollView {
        anchors.fill: parent; contentWidth: availableWidth
        ColumnLayout {
            width: parent.width; spacing: 12

            // ===== ROW 1: Phone+Buttons | Gauges+Detail =====
            RowLayout {
                Layout.fillWidth: true; Layout.leftMargin: 16; Layout.rightMargin: 16; Layout.topMargin: 8
                spacing: 14

                // --- LEFT: Phone + buttons ---
                ColumnLayout {
                    spacing: 0

                    Rectangle {
                        Layout.preferredWidth: phoneW + 72; Layout.preferredHeight: phoneH
                        radius: 16; border.color: FluTheme.dark ? "#333" : "#ddd"; border.width: 1
                        color: FluTheme.dark ? "#1a1a1e" : "#f0f0f3"

                        RowLayout {
                            anchors { fill: parent; margins: 8 }
                            spacing: 6

                            Rectangle {
                                Layout.preferredWidth: phoneW; Layout.fillHeight: true; radius: 10; color: "#000"
                                MirrorScene {
                                    id: mirrorView; anchors { fill: parent; margins: 1 }
                                    ImageFrameItem { imageFrame: mirrorView.image; hasAlphaChannel: false; anchors.fill: parent }
                                }
                                Rectangle {
                                    visible: !mirrorActive; anchors.fill: parent; color: "#0a0a0a"
                                    ColumnLayout {
                                        anchors.centerIn: parent; spacing: 4
                                        FluText { text: device ? (device.model || "设备") : "无设备"; color: "#555"; font: FluTextStyle.Caption; Layout.alignment: Qt.AlignHCenter }
                                    }
                                }
                            }

                            ColumnLayout {
                                Layout.fillHeight: true; Layout.preferredWidth: 50; spacing: 2
                                FluButton { text: "主页";  Layout.fillWidth: true; Layout.fillHeight: true; onClicked: DeviceControl.control(ADT.Key, ADT.Home) }
                                FluButton { text: "电源"; Layout.fillWidth: true; Layout.fillHeight: true; onClicked: DeviceControl.control(ADT.Key, ADT.Power) }
                                FluButton { text: "音量+";  Layout.fillWidth: true; Layout.fillHeight: true; onClicked: DeviceControl.control(ADT.Music, ADT.VolumeAdd) }
                                FluButton { text: "音量-";  Layout.fillWidth: true; Layout.fillHeight: true; onClicked: DeviceControl.control(ADT.Music, ADT.VolumeReduce) }
                                FluButton { text: "返回";  Layout.fillWidth: true; Layout.fillHeight: true; onClicked: DeviceControl.control(ADT.Key, ADT.Back) }
                                FluButton { text: "菜单";  Layout.fillWidth: true; Layout.fillHeight: true; onClicked: DeviceControl.control(ADT.Key, ADT.Menu) }
                                FluButton { text: "截图";  Layout.fillWidth: true; Layout.fillHeight: true; onClicked: DeviceControl.control(ADT.Key, ADT.Shot) }
                                FluButton { text: "静音";  Layout.fillWidth: true; Layout.fillHeight: true; onClicked: DeviceControl.control(ADT.Key, ADT.Mute) }
                                FluButton { text: "重启";Layout.fillWidth: true; Layout.fillHeight: true; onClicked: DeviceControl.control(ADT.Key, ADT.Reboot) }
                                FluButton { text: "关机";  Layout.fillWidth: true; Layout.fillHeight: true; onClicked: DeviceControl.control(ADT.Key, ADT.Poweroff) }
                                FluButton { text: "恢复";   Layout.fillWidth: true; Layout.fillHeight: true; onClicked: DeviceControl.control(ADT.Key, ADT.RebootToRec) }
                                FluButton { text: "引导";    Layout.fillWidth: true; Layout.fillHeight: true; onClicked: DeviceControl.control(ADT.Key, ADT.RebootToFB) }
                            }
                        }
                    }

                    RowLayout {
                        Layout.fillWidth: true; spacing: 6; Layout.topMargin: 8
                        FluButton { text: mirrorActive ? "停止" : "投屏"; Layout.fillWidth: true; onClicked: toggleMirror() }
                        FluButton { text: "截图"; Layout.fillWidth: true; onClicked: ImageDetailTools.shotScreen("") }
                        FluButton { text: "帧率"; Layout.fillWidth: true; onClicked: fpsPopup.open() }
                    }
                }

                // --- RIGHT: Gauges + Detail ---
                ColumnLayout {
                    Layout.fillWidth: true; Layout.fillHeight: true; spacing: 10; visible: !!device

                    FluFrame {
                        Layout.fillWidth: true
                        RowLayout {
                            anchors.left: parent.left
                            anchors.right: parent.right
                            ColumnLayout {
                                Layout.fillWidth: true
                                Layout.topMargin: 10
                                Layout.bottomMargin: 10
                                Layout.leftMargin: 10
                                Layout.rightMargin: 10
                                spacing: 10

                                RowLayout {
                                    FluText { text: device ? (device.manufacturer || "") + " " + (device.model || "") : ""; font: FluTextStyle.Subtitle; Layout.fillWidth: true }
                                    FluText { text: "Android " + (device ? (device.androidVersion || "") : ""); font: FluTextStyle.Caption; color: FluTheme.fontSecondaryColor }
                                }

                                Rectangle {
                                    Layout.fillWidth: true; Layout.preferredHeight: 30; radius: 15
                                    color: FluTheme.dark ? Qt.rgba(1,1,1,0.06) : Qt.rgba(0,0,0,0.04)
                                    Rectangle {
                                        height: parent.height; radius: 15
                                        width: Math.max(0, parent.width * (device ? device.batteryLevel / 100.0 : 0))
                                        gradient: Gradient {
                                            GradientStop { position: 0; color: Qt.hsla(0.33 - (device?device.batteryLevel/300:0), 0.7, 0.45, 1) }
                                            GradientStop { position: 1; color: Qt.hsla(0.33 - (device?device.batteryLevel/300:0), 0.9, 0.38, 1) }
                                        }
                                        Behavior on width { SmoothedAnimation { duration: 600 } }
                                    }
                                    RowLayout {
                                        anchors { fill: parent; leftMargin: 14; rightMargin: 14 }
                                        FluText { text: (device?device.batteryLevel:0)+"%"; font: FluTextStyle.CaptionStrong; color: (device&&device.batteryLevel>50)?"#fff":FluTheme.fontPrimaryColor }
                                        Item { Layout.fillWidth: true }
                                        FluText { visible: device && device.chargingType !== ADT.None; text: "充电中"; font: FluTextStyle.Caption; color: (device&&device.batteryLevel>50)?Qt.rgba(1,1,1,0.7):FluTheme.fontSecondaryColor }
                                        FluText { text: battTemp.toFixed(1)+"°C  "+(device?(device.batteryVoltage/1000.0).toFixed(1)+"V":""); font: FluTextStyle.Caption; color: (device&&device.batteryLevel>50)?Qt.rgba(1,1,1,0.7):FluTheme.fontSecondaryColor }
                                    }
                                }

                                FluDivider { Layout.fillWidth: true }

                                GridLayout {
                                    columns: 2; columnSpacing: 32; rowSpacing: 10; Layout.fillWidth: true
                                    InfoCell { icon: "cpu";    label: "CPU";        value: device ? (device.cpuInfo||"-") : "-" }
                                    InfoCell { icon: "cores";  label: "核心/频率";   value: (device ? (device.maxCoreNum||"?") : "?") + " · " + (device ? (device.maxFreq||"?") : "?") }
                                    InfoCell { icon: "memory"; label: "内存";        value: (device ? (device.memory||"-") : "-") + "  |  " + SystemInfo.ramUsage.toFixed(1) + "/" + SystemInfo.ramTotal.toFixed(1) + " GB" }
                                    InfoCell { icon: "storage";label: "存储";        value: SystemInfo.storageUsed.toFixed(0) + " / " + SystemInfo.storageTotal.toFixed(0) + " GB" }
                                    InfoCell { icon: "resolution";label: "分辨率";   value: (device ? (device.resolution||"-") : "-") + " · " + (device ? (device.dpi||"-") : "-") + " DPI" }
                                    InfoCell { icon: "serial"; label: "序列号";      value: device ? (device.serialNumber||"-") : "-" }
                                    InfoCell { icon: "device"; label: "设备/代号";   value: (device ? (device.deviceName||"?") : "?") + " / " + (device ? (device.deviceCode||"?") : "?") }
                                    InfoCell { icon: "system"; label: "系统";        value: device ? ((device.systemInfo||"") + "  SDK " + (device.sdkVersion||"")) : "-" }
                                    InfoCell { icon: "ip";     label: "IP";          value: device ? (device.ipAddr||"-") : "-" }
                                    InfoCell { icon: "app";    label: "前台";        value: device && device.currentPackage ? device.currentPackage : "-" }
                                }
                            }
                        }
                    }

                    FluFrame {
                        Layout.fillWidth: true
                        Layout.bottomMargin: 10
                        ColumnLayout {
                            anchors.fill: parent
                            RowLayout {
                                Layout.alignment: Qt.AlignHCenter
                                Layout.topMargin: 10
                                Layout.bottomMargin: 10
                                CircleGauge { value: SystemInfo.cpuUsage; label: "CPU"; subtitle: device ? (device.cpuInfo || "") : ""; implicitWidth: 100; implicitHeight: 115 }
                                CircleGauge { value: SystemInfo.gpuUsage; label: "GPU"; implicitWidth: 100; implicitHeight: 115 }
                                CircleGauge { value: ramPct; label: "RAM"; subtitle: SystemInfo.ramUsage.toFixed(1)+"/"+SystemInfo.ramTotal.toFixed(1)+"G"; implicitWidth: 100; implicitHeight: 115 }
                                CircleGauge { value: SystemInfo.cpuTemp; label: "Temp"; minValue: 20; maxValue: 100; unit: "°C"; subtitle: battTemp.toFixed(1)+"°C"; implicitWidth: 100; implicitHeight: 115 }
                            }
                            Item {
                                Layout.fillWidth: true
                                Layout.preferredHeight: 4
                            }
                        }
                    }
                }
            }

            // --- No device ---
            FluFrame {
                Layout.fillWidth: true; Layout.preferredHeight: 160; Layout.leftMargin: 16; Layout.rightMargin: 16; visible: !device
                ColumnLayout { anchors.centerIn: parent; spacing: 12
                    FluText { text: "未连接设备"; font: FluTextStyle.Title; Layout.alignment: Qt.AlignHCenter }
                    FluText { text: "请连接设备后开始监控"; color: FluTheme.fontSecondaryColor; Layout.alignment: Qt.AlignHCenter }
                    RowLayout { Layout.alignment: Qt.AlignHCenter; spacing: 8
                        FluButton { text: "刷新"; onClicked: ConnectManager.startCheckDevice() }
                        FluButton { text: "自动连接"; onClicked: ConnectManager.startADBServer() }
                    }
                }
            }

            // --- FluentUI Pivot Controls ---
            FluFrame {
                Layout.fillWidth: true; Layout.leftMargin: 16; Layout.rightMargin: 16; visible: !!device
                Layout.preferredHeight: 210

                FluPivot {
                    font.pixelSize: 14
                    anchors.fill: parent
                    anchors.leftMargin: 12
                    anchors.rightMargin: 12
                    currentIndex: 0
                    headerHeight: 36

                    FluPivotItem {
                        title: "媒体控制"
                        contentItem: Component {
                            GridLayout {
                                anchors { fill: parent; margins: 12 }
                                columns: 3; columnSpacing: 6; rowSpacing: 6
                                Repeater { model: ["上一曲","停止","播放/暂停","下一曲","音量-","音量+"]
                                    FluButton { text: modelData; Layout.fillWidth: true; onClicked: DeviceControl.control(ADT.Music, index) } }
                            }
                        }
                    }
                    FluPivotItem {
                        title: "按键模拟"
                        contentItem: Component {
                            GridLayout {
                                anchors { fill: parent; margins: 12 }
                                columns: 5; columnSpacing: 6; rowSpacing: 6
                                Repeater { model: ["菜单","主页","返回","电源","亮度+","亮度-","关机","重启","恢复","引导","截图","静音","助手","主页↑","结束↓"]
                                    FluButton { text: modelData; Layout.fillWidth: true; onClicked: DeviceControl.control(ADT.Key, index) } }
                            }
                        }
                    }
                    FluPivotItem {
                        title: "广播模拟"
                        contentItem: Component {
                            GridLayout {
                                anchors { fill: parent; margins: 12 }
                                columns: 4; columnSpacing: 6; rowSpacing: 6
                                Repeater { model: ["网络状态","亮屏","息屏","电量低","电量正常","启动完成","存储低","存储正常","安装","WiFi1","WiFi2","充电状态","输入法","上电","断电","休眠开","休眠关","壁纸","耳机","卸载","挂载","省电"]
                                    FluButton { text: modelData; Layout.fillWidth: true; onClicked: DeviceControl.control(ADT.Broadcast, index) } }
                            }
                        }
                    }
                    FluPivotItem {
                        title: "电池伪装"
                        contentItem: Component {
                            ColumnLayout {
                                anchors { fill: parent; margins: 12 }
                                spacing: 8
                                RowLayout {
                                    FluTextBox { id: batLevel; placeholderText: "电量"; text: "100"; Layout.preferredWidth: 80 }
                                    FluButton { text: "设置"; onClicked: device.setBatteryLevel(parseInt(batLevel.text)||100) }
                                }
                                RowLayout {
                                    FluButton { text: "停止充电"; Layout.fillWidth: true; onClicked: device.stopCharge() }
                                    FluButton { text: "USB不充电"; Layout.fillWidth: true; onClicked: device.connectButNoCharge() }
                                    FluButton { text: "恢复"; Layout.fillWidth: true; onClicked: device.restoreCharge() }
                                    FluButton { text: "全部重置"; Layout.fillWidth: true; onClicked: device.restoreBatteryAll() }
                                }
                            }
                        }
                    }
                    FluPivotItem {
                        title: "分辨率/DPI"
                        contentItem: Component {
                            GridLayout {
                                anchors { fill: parent; margins: 12 }
                                columns: 3; columnSpacing: 8; rowSpacing: 8
                                FluTextBox { id: resW; placeholderText: "宽度"; text: device ? (device.resolution.split('x')[0]||"1080") : "1080"; Layout.fillWidth: true }
                                FluTextBox { id: resH; placeholderText: "高度"; text: device ? (device.resolution.split('x')[1]||"1920") : "1920"; Layout.fillWidth: true }
                                FluTextBox { id: dpiVal; placeholderText: "DPI"; text: device ? (device.dpi||"420") : "420"; Layout.fillWidth: true }
                                FluButton { text: "恢复"; Layout.fillWidth: true; onClicked: device.restoreResolutionAndDpi() }
                                FluButton { text: "应用"; Layout.fillWidth: true; Layout.columnSpan: 2
                                    onClicked: { var w = parseInt(resW.text), h = parseInt(resH.text), d = parseInt(dpiVal.text); if (w&&h) device.setResolution(w,h); if (d) device.setDPI(d) }
                                }
                            }
                        }
                    }
                    FluPivotItem {
                        title: "文件传输"
                        contentItem: Component {
                            ColumnLayout {
                                anchors { fill: parent; margins: 12 }
                                spacing: 8
                                RowLayout {
                                    FluTextBox { id: fileSrc; placeholderText: "本地路径"; Layout.fillWidth: true }
                                    FluButton { text: "浏览"; onClicked: fileDlg.open() }
                                }
                                RowLayout {
                                    FluTextBox { id: fileDst; placeholderText: "设备路径"; text: "/sdcard/"; Layout.fillWidth: true }
                                    FluButton { text: "推送"; onClicked: device.pushFile(fileSrc.text, fileDst.text) }
                                }
                                RowLayout {
                                    FluTextBox { id: pullSrc; placeholderText: "设备文件"; Layout.fillWidth: true }
                                    FluTextBox { id: pullDst; placeholderText: "本地目录"; Layout.fillWidth: true }
                                    FluButton { text: "拉取"; onClicked: device.extractFile(pullSrc.text, pullDst.text) }
                                }
                            }
                        }
                    }
                    FluPivotItem {
                        title: "启动活动"
                        contentItem: Component {
                            ColumnLayout {
                                anchors { fill: parent; margins: 12 }
                                spacing: 8
                                RowLayout {
                                    FluTextBox { id: actName; placeholderText: "Activity名称"; text: "com.tencent.mm/.ui.LauncherUI"; Layout.fillWidth: true }
                                    FluTextBox { id: actArgs; placeholderText: "参数"; Layout.fillWidth: true }
                                    FluButton { text: "启动"; onClicked: device.startActivity(actName.text, actArgs.text ? [actArgs.text] : []) }
                                }
                            }
                        }
                    }
                }
            }

            Item { Layout.preferredHeight: 8 }
        }
    }

    FileDialog { id: fileDlg; title: "选择文件"; fileMode: FileDialog.OpenFile; onAccepted: fileSrc.text = String(currentFile) }
}
