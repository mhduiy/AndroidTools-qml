import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import MFloat

import "../components"
import NotificationController 1.0
import DeviceControl 1.0
import App
import ConnectManager

ItemPage {
    id: root
    state: "pageShow"
    ColorConstants {
        id: colorConstants
    }

    property var currentDevice: ConnectManager.cutADBDevice

    RowLayout {
        anchors.fill: parent
        MWrapper {
            Layout.preferredWidth: parent.width * 0.48
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft
            ColumnLayout {
                RowLayout {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 200
                    spacing: 10
                    Rectangle {
                        Layout.preferredHeight: 200
                        Layout.preferredWidth: 130
                        radius: 12
                        color: "transparent"

                        BatteryRect {
                            anchors.fill: parent
                            level: currentDevice ? currentDevice.batteryLevel : 0
                        }
                    }

                    ColumnLayout {
                        spacing: 5
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.alignment: Qt.AlignLeft
                        Label {
                            text: {
                                if (!currentDevice) {
                                    return "请连接设备"
                                }
                                
                                let title = currentDevice.manufacturer || ""
                                if (title === "") {
                                    return "请连接设备"
                                }
                                
                                // 优先显示设备名，其次是型号
                                let deviceName = currentDevice.deviceName || currentDevice.model || ""
                                if (deviceName !== "") {
                                    title += " " + deviceName
                                }
                                
                                return title
                            }
                            font.pixelSize: 16
                            font.bold: true
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }

                        Item {
                            Layout.preferredHeight: 10
                            Layout.fillWidth: true
                        }

                        Repeater {
                            model: [
                                {
                                    key: "设备序列号:",
                                    value: currentDevice ? currentDevice.serialNumber : ""
                                },
                                {
                                    key: "CPU:",
                                    value: currentDevice ? currentDevice.cpuInfo : ""
                                },
                                {
                                    key: "安卓版本:",
                                    value: currentDevice ? currentDevice.androidVersion : ""
                                },
                                {
                                    key: "分辨率:",
                                    value: currentDevice ? currentDevice.resolution : ""
                                },
                            ]
                            RowLayout {
                                Label {
                                    text: modelData.key
                                    color: App.windowTextAssitColor
                                }
                                Label {
                                    text: modelData.value
                                    color: App.windowTextAssitColor
                                }
                            }
                        }

                        GridLayout {
                            columns: 2
                            uniformCellHeights: true
                            uniformCellWidths: true
                            Layout.topMargin: 2
                            columnSpacing: 10
                            rowSpacing: 3
                            MButton {
                                text: "关机"
                                Layout.fillWidth: true
                                btnType: MButton.FBtnType.Warning
                                enabled: currentDevice !== null
                                onClicked: {
                                    if (currentDevice) {
                                        currentDevice.control(1, 6) // CTRL_Key, Poweroff
                                    }
                                }
                            }
                            MButton {
                                text: "重启"
                                Layout.fillWidth: true
                                btnType: MButton.FBtnType.Suggest
                                enabled: currentDevice !== null
                                onClicked: {
                                    if (currentDevice) {
                                        currentDevice.control(1, 7) // CTRL_Key, Reboot
                                    }
                                }
                            }
                            MButton {
                                text: "Recovery"
                                Layout.fillWidth: true
                                btnType: MButton.FBtnType.Ordinary
                                enabled: currentDevice !== null
                                onClicked: {
                                    if (currentDevice) {
                                        currentDevice.control(1, 8) // CTRL_Key, RebootToRec
                                    }
                                }
                            }
                            MButton {
                                text: "FastBoot"
                                Layout.fillWidth: true
                                btnType: MButton.FBtnType.Ordinary
                                enabled: currentDevice !== null
                                onClicked: {
                                    if (currentDevice) {
                                        currentDevice.control(1, 9) // CTRL_Key, RebootToFB
                                    }
                                }
                            }
                        }
                    }
                }

                Item {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    ScrollView {
                        id: scrollView
                        anchors.fill: parent
                        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
                        ScrollBar.vertical.policy: ScrollBar.AlwaysOff
                        ColumnLayout {
                            width: scrollView.availableWidth
                            spacing: 10

                            Repeater {
                                id: deviceDetailInfoRep
                                property var texts: [
                                    ["厂商", "品牌", "型号", "设备名", "设备代号"], 
                                    ["CPU信息", "最大频率", "核心数"], 
                                    ["系统", "安卓版本", "SDK版本"], 
                                    ["分辨率", "DPI"], 
                                    ["MAC地址", "IP地址", "内存容量"]
                                ]
                                property var values: [
                                    [
                                        currentDevice ? currentDevice.manufacturer : "",
                                        currentDevice ? currentDevice.brand : "",
                                        currentDevice ? currentDevice.model : "",
                                        currentDevice ? currentDevice.deviceName : "",
                                        currentDevice ? currentDevice.deviceCode : ""
                                    ],
                                    [
                                        currentDevice ? currentDevice.cpuInfo : "",
                                        currentDevice ? currentDevice.maxFreq : "",
                                        currentDevice ? currentDevice.maxCoreNum : ""
                                    ],
                                    [
                                        currentDevice ? currentDevice.systemInfo : "未知",
                                        currentDevice ? currentDevice.androidVersion : "",
                                        currentDevice ? currentDevice.sdkVersion : ""
                                    ],
                                    [
                                        currentDevice ? currentDevice.resolution : "",
                                        currentDevice ? currentDevice.dpi : ""
                                    ],
                                    [
                                        currentDevice ? currentDevice.macAddr : "",
                                        currentDevice ? currentDevice.ipAddr : "",
                                        currentDevice ? currentDevice.memory : ""
                                    ]
                                ]
                                property var titles: ["设备信息", "处理器", "系统信息", "屏幕", "其他信息"]
                                model: texts.length
                                ColumnLayout {
                                    Label {
                                        text: deviceDetailInfoRep.titles[index]
                                        font.bold: true
                                        font.pixelSize: 16
                                        color: App.windowTextColor
                                    }
                                    Repeater {
                                        id: rep
                                        model: deviceDetailInfoRep.texts[index].length
                                        property int repIndex: index
                                        delegate: RowLayout {
                                            Layout.leftMargin: 20
                                            Rectangle {
                                                width: 10
                                                height: 10
                                                radius: width / 2
                                                color: "#008c8c"
                                                opacity: 0.6
                                            }

                                            Item {
                                                Layout.preferredWidth: 5
                                                Layout.preferredHeight: 30
                                            }

                                            Label {
                                                text: deviceDetailInfoRep.texts[rep.repIndex][index]
                                                font.pixelSize: 14
                                            }

                                            Rectangle {
                                                Layout.fillWidth: true
                                                Layout.preferredHeight: 1
                                                color: App.windowTextAssitColor
                                            }

                                            Label {
                                                visible: deviceDetailInfoRep.values[rep.repIndex][index] !== ""
                                                text: deviceDetailInfoRep.values[rep.repIndex][index]
                                            }

                                            MLoadIndicator {
                                                scale: 0.8
                                                visible: deviceDetailInfoRep.values[rep.repIndex][index] === ""
                                                Component.onCompleted: {
                                                    start();
                                                }
                                            }
                                        }
                                    }
                                }
                            }

                            Item {
                                Layout.fillHeight: true
                            }
                        }
                    }
                }
            }
        }
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignTop

            ScrollView {
                id: scrollViewRight
                Layout.fillWidth: true
                Layout.fillHeight: true
                ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
                ScrollBar.vertical.policy: ScrollBar.AlwaysOff
                ColumnLayout {
                    width: parent.parent.width
                    MWrapper {
                        // 当前活动信息
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        ColumnLayout {
                            anchors.fill: parent
                            RowLayout {
                                Layout.fillWidth: true
                                Layout.preferredHeight: 30
                                Label {
                                    font.bold: true
                                    font.pixelSize: 16
                                    text: "前台应用"
                                }
                                Item {
                                    Layout.fillWidth: true
                                }

                                MButton {
                                    Layout.preferredWidth: 80
                                    text: "强行停止"
                                    btnType: MButton.FBtnType.Warning
                                    enabled: currentDevice !== null && currentDevice.currentPackage !== ""
                                    onClicked: {
                                        if (currentDevice && currentDevice.currentPackage !== "") {
                                            currentDevice.killActivity(currentDevice.currentPackage)
                                            NotificationController.send("命令已发送", "当前应用已停止", 1, 3000);
                                        }
                                    }
                                }
                            }
                            GridLayout {
                                columns: 2
                                Layout.fillHeight: true
                                Layout.fillWidth: true
                                MLabel {
                                    Layout.preferredWidth: 100
                                    rectColor: colorConstants.suggestClickedColor
                                    text: "窗口标识符"
                                }
                                Label {
                                    Layout.fillWidth: true
                                    text: currentDevice ? currentDevice.windowCode : ""
                                }
                                MLabel {
                                    Layout.preferredWidth: 100
                                    rectColor: colorConstants.suggestClickedColor
                                    text: "前台包名"
                                }
                                Label {
                                    Layout.fillWidth: true
                                    text: currentDevice ? currentDevice.currentPackage : ""
                                }
                                MLabel {
                                    Layout.preferredWidth: 100
                                    rectColor: colorConstants.suggestClickedColor
                                    text: "前台活动"
                                }
                                Label {
                                    Layout.fillWidth: true
                                    text: currentDevice ? currentDevice.currentActivity : ""
                                }
                            }
                        }
                    }
                    MonitorItem {
                        title: "电池电量 (" + cutValue + "%)"
                        minY: 0
                        maxY: 100
                        cutValue: currentDevice ? currentDevice.batteryLevel : 0
                    }
                    MonitorItem {
                        title: "电池温度 (" + cutValue + "°C)"
                        minY: 20
                        maxY: 50
                        cutValue: currentDevice ? currentDevice.batteryTemperature : 0
                    }
                    MonitorItem {
                        title: "电池电压 (" + cutValue + "mV)"
                        minY: 3000
                        maxY: 5000
                        cutValue: currentDevice ? currentDevice.batteryVoltage : 0
                    }
                    MonitorItem {
                        title: "电池电流 (" + cutValue + "mA)"
                        minY: 0
                        maxY: 3000
                        cutValue: currentDevice ? currentDevice.batteryCurrent : 0
                    }
                }
            }
        }
    }
}

