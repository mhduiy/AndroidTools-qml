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
    title: "Device Info"

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
                Layout.preferredHeight: 180
                Layout.leftMargin: 20
                Layout.rightMargin: 20
                Layout.topMargin: 12

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 20
                    spacing: 28

                    BatteryCanvas {
                        Layout.preferredWidth: 120
                        Layout.preferredHeight: 160
                        level: device ? device.batteryLevel : 0
                        charging: device ? device.chargingType !== ADT.None : false
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 6

                        FluText {
                            text: device ? device.model || "Unknown" : "No device connected"
                            font: FluTextStyle.Title
                        }
                        FluText {
                            text: device ? (device.manufacturer || "") + "  ·  " + (device.brand || "") : "Connect via USB or wireless"
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
                                    { l: "Cores", v: device ? device.maxCoreNum || "--" : "--" },
                                    { l: "Memory", v: device ? device.memory || "--" : "--" },
                                    { l: "Resolution", v: device ? device.resolution || "--" : "--" }
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
                Layout.leftMargin: 20
                Layout.rightMargin: 20

                GridLayout {
                    anchors.fill: parent
                    anchors.margins: 20
                    columns: 3
                    columnSpacing: 32
                    rowSpacing: 6

                    Repeater {
                        model: [
                            { l: "Brand", v: device ? device.brand : "--" },
                            { l: "Model", v: device ? device.model : "--" },
                            { l: "Device", v: device ? device.deviceName : "--" },
                            { l: "Codename", v: device ? device.deviceCode : "--" },
                            { l: "System", v: device ? device.systemInfo : "--" },
                            { l: "SDK", v: device ? device.sdkVersion : "--" },
                            { l: "Max Freq", v: device ? device.maxFreq : "--" },
                            { l: "DPI", v: device ? device.dpi : "--" },
                            { l: "Serial", v: device ? device.serialNumber : "--" },
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
                    FluText { text: "Quick Actions"; font: FluTextStyle.BodyStrong; Layout.alignment: Qt.AlignHCenter }
                    RowLayout {
                        spacing: 8; Layout.alignment: Qt.AlignHCenter
                        FluButton { text: "Shutdown"; enabled: !!device; onClicked: DeviceControl.control(ADT.Key, ADT.Poweroff) }
                        FluButton { text: "Reboot"; enabled: !!device; onClicked: DeviceControl.control(ADT.Key, ADT.Reboot) }
                        FluButton { text: "Recovery"; enabled: !!device; onClicked: DeviceControl.control(ADT.Key, ADT.RebootToRec) }
                        FluButton { text: "Fastboot"; enabled: !!device; onClicked: DeviceControl.control(ADT.Key, ADT.RebootToFB) }
                    }
                }
            }
        }
    }
}
