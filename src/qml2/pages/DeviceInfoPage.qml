import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import FluentUI
import ConnectManager 1.0
import DeviceControl 1.0
import ADT 1.0
import "qrc:/qml2/components"

FluContentPage {
    id: page
    title: "设备信息"

    property var device: ConnectManager.cutADBDevice

    ScrollView {
        anchors.fill: parent
        contentWidth: availableWidth

        ColumnLayout {
            width: parent.width
            spacing: 12

            // ---- 电池 + 概览 ----
            FluFrame {
                Layout.fillWidth: true
                Layout.preferredHeight: 210
                Layout.leftMargin: 20
                Layout.rightMargin: 20
                Layout.topMargin: 12

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 20
                    spacing: 28

                    BatteryCanvas {
                        Layout.preferredWidth: 120
                        Layout.preferredHeight: 170
                        level: device ? device.batteryLevel : 0
                        charging: device ? device.chargingType !== ADT.None : false
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 6

                        FluText {
                            text: device ? device.model || "未知" : "未连接设备"
                            font: FluTextStyle.Title
                        }
                        FluText {
                            text: device ? (device.manufacturer || "") + "  ·  " + (device.brand || "") : "通过USB或无线连接"
                            font: FluTextStyle.Body
                            color: FluTheme.fontSecondaryColor
                        }
                        FluText {
                            text: device ? "Android " + (device.androidVersion || "") : ""
                            font: FluTextStyle.Body
                            color: FluTheme.fontSecondaryColor
                        }

                        Item { Layout.fillHeight: true }

                        RowLayout {
                            spacing: 32
                            Repeater {
                                model: [
                                    { l: "SOC", v: device ? device.cpuInfo || "--" : "--" },
                                    { l: "核心", v: device ? device.maxCoreNum || "--" : "--" },
                                    { l: "内存", v: device ? device.memory || "--" : "--" },
                                    { l: "分辨率", v: device ? device.resolution || "--" : "--" }
                                ]
                                ColumnLayout {
                                    spacing: 2
                                    FluText { text: modelData.v; font: FluTextStyle.BodyStrong; Layout.alignment: Qt.AlignHCenter }
                                    FluText { text: modelData.l; font: FluTextStyle.Caption; color: FluTheme.fontSecondaryColor; Layout.alignment: Qt.AlignHCenter }
                                }
                            }
                        }
                    }
                }
            }

            // ---- 详细信息 ----
            FluFrame {
                Layout.fillWidth: true
                Layout.preferredHeight: 230
                Layout.leftMargin: 20
                Layout.rightMargin: 20

                GridLayout {
                    anchors { left: parent.left; top: parent.top; right: parent.right; margins: 20 }
                    columns: 2
                    columnSpacing: 32
                    rowSpacing: 8

                    Repeater {
                        model: [
                            { l: "品牌", v: device ? device.brand : "--" },
                            { l: "型号", v: device ? device.model : "--" },
                            { l: "设备名", v: device ? device.deviceName : "--" },
                            { l: "代号", v: device ? device.deviceCode : "--" },
                            { l: "系统", v: device ? device.systemInfo : "--" },
                            { l: "SDK", v: device ? device.sdkVersion : "--" },
                            { l: "最大频率", v: device ? device.maxFreq : "--" },
                            { l: "DPI", v: device ? device.dpi : "--" },
                            { l: "序列号", v: device ? device.serialNumber : "--" },
                        ]
                        RowLayout {
                            spacing: 6; Layout.fillWidth: true
                            FluText { text: modelData.l + ":"; font: FluTextStyle.Caption; color: FluTheme.fontSecondaryColor; Layout.preferredWidth: 80 }
                            FluText { text: modelData.v || "--"; font: FluTextStyle.Body; Layout.fillWidth: true; elide: Text.ElideRight }
                        }
                    }
                }
            }

            // ---- 快捷操作 ----
            FluFrame {
                Layout.fillWidth: true
                Layout.preferredHeight: 100
                Layout.leftMargin: 20
                Layout.rightMargin: 20

                ColumnLayout {
                    anchors.centerIn: parent; spacing: 10
                    FluText { text: "快捷操作"; font: FluTextStyle.BodyStrong; Layout.alignment: Qt.AlignHCenter }
                    Flow {
                        spacing: 8; Layout.alignment: Qt.AlignHCenter
                        FluButton { text: "关机"; enabled: !!device; onClicked: DeviceControl.control(ADT.Key, ADT.Poweroff) }
                        FluButton { text: "重启"; enabled: !!device; onClicked: DeviceControl.control(ADT.Key, ADT.Reboot) }
                        FluButton { text: "恢复"; enabled: !!device; onClicked: DeviceControl.control(ADT.Key, ADT.RebootToRec) }
                        FluButton { text: "引导"; enabled: !!device; onClicked: DeviceControl.control(ADT.Key, ADT.RebootToFB) }
                    }
                }
            }
        }
    }
}
