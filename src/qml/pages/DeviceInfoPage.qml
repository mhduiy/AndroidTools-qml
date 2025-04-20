import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import MFloat

import "../components"
import BatteryControl 1.0
import CutActivityControl 1.0
import ADBControl 1.0
import DetailInfoControl 1.0
import NotificationController 1.0
import DeviceControl 1.0
import RealTimeInfoHelper 1.0
import App
import ConnectManager

ItemPage {
    id: root
    state: "pageShow"
    ColorConstants {
        id: colorConstants
    }

    function gInfo(key) {
        return DetailInfoControl.info[key];
    }

    property var monitorItems: [0, 1]

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
                            level: BatteryControl.level
                        }
                    }

                    ColumnLayout {
                        spacing: 5
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.alignment: Qt.AlignLeft
                        Label {
                            text: {
                                let title
                                if (ConnectManager.currentDeviceCode === "") {
                                    title = "请连接设备"
                                } else {
                                    title = DetailInfoControl.info[DetailInfoControl.DETA_MANUFACTURER];
                                    if (title === "" || title === undefined) {
                                        title = "请连接设备";
                                        return title;
                                    }
                                    title = title + " " + (DetailInfoControl.info[DetailInfoControl.DETA_DEVICENAME] === "" ? DetailInfoControl.info[DetailInfoControl.DETA_MODEL] : DetailInfoControl.info[DetailInfoControl.DETA_DEVICENAME]);
                                }

                                return title;
                            }
                            font.pixelSize: 22
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
                                    value: DetailInfoControl.info[DetailInfoControl.DETA_SERIALNUMBER]
                                },
                                {
                                    key: "CPU:",
                                    value: DetailInfoControl.info[DetailInfoControl.DETA_CPUINFO]
                                },
                                {
                                    key: "安卓版本:",
                                    value: DetailInfoControl.info[DetailInfoControl.DETA_ANDROIDVERSION]
                                },
                                {
                                    key: "分辨率:",
                                    value: DetailInfoControl.info[DetailInfoControl.DETA_RESOLVING]
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
                                onClicked: {
                                    DeviceControl.control(DeviceControl.CTRL_Key, DeviceControl.Poweroff)
                                }
                            }
                            MButton {
                                text: "重启"
                                Layout.fillWidth: true
                                btnType: MButton.FBtnType.Suggest
                                onClicked: {
                                    DeviceControl.control(DeviceControl.CTRL_Key, DeviceControl.Reboot)
                                }
                            }
                            MButton {
                                text: "Recovery"
                                Layout.fillWidth: true
                                btnType: MButton.FBtnType.Ordinary
                                onClicked: {
                                    DeviceControl.control(DeviceControl.CTRL_Key, DeviceControl.RebootToRec)
                                }
                            }
                            MButton {
                                text: "FastBoot"
                                Layout.fillWidth: true
                                btnType: MButton.FBtnType.Ordinary
                                onClicked: {
                                    DeviceControl.control(DeviceControl.CTRL_Key, DeviceControl.RebootToFB)
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
                            // 设备Title
                            spacing: 10

                            Repeater {
                                id: deviceDetailInfoRep
                                property var texts: [["厂商", "品牌", "型号", "设备名", "设备代号"], ["CPU信息", "最大频率", "核心数"], ["系统", "安卓版本", "sdk版本"], ["尺寸", "分辨率", "DPI"], ["MAC地址", "IP地址", "内存容量"]]
                                property var values: [[DetailInfoControl.info[DetailInfoControl.DETA_MANUFACTURER], DetailInfoControl.info[DetailInfoControl.DETA_BRAND], DetailInfoControl.info[DetailInfoControl.DETA_MODEL], DetailInfoControl.info[DetailInfoControl.DETA_DEVICENAME], DetailInfoControl.info[DetailInfoControl.DETA_DEVICECODE]], [DetailInfoControl.info[DetailInfoControl.DETA_CPUINFO], DetailInfoControl.info[DetailInfoControl.DATE_MAXFREP], DetailInfoControl.info[DetailInfoControl.DATE_MAXCORENUM]], ["未知", DetailInfoControl.info[DetailInfoControl.DETA_ANDROIDVERSION], DetailInfoControl.info[DetailInfoControl.DETA_SDKVERSION]], ["未知", DetailInfoControl.info[DetailInfoControl.DETA_RESOLVING], DetailInfoControl.info[DetailInfoControl.DETA_DPI]], [DetailInfoControl.info[DetailInfoControl.DETA_MACADDR], DetailInfoControl.info[DetailInfoControl.DETA_IPADDR], DetailInfoControl.info[DetailInfoControl.DETA_MEMORY]]]
                                property var titles: ["设备信息", "处理器", "系统信息", "屏幕", "其他信息"]
                                model: texts.length
                                ColumnLayout {
                                    Label {
                                        text: deviceDetailInfoRep.titles[index]
                                        font.bold: true
                                        font.pixelSize: 18
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
                                                visible: deviceDetailInfoRep.values[rep.repIndex][index] !== undefined
                                                text: deviceDetailInfoRep.values[rep.repIndex][index]
                                            }

                                            MLoadIndicator {
                                                scale: 0.8
                                                visible: deviceDetailInfoRep.values[rep.repIndex][index] === undefined
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
                                    font.pixelSize: 20
                                    text: "前台应用"
                                }
                                Item {
                                    Layout.fillWidth: true
                                }

                                MButton {
                                    Layout.preferredWidth: 80
                                    text: "强行停止"
                                    btnType: MButton.FBtnType.Warning
                                    onClicked: {
                                        CutActivityControl.killCutActivity();
                                        NotificationController.send("命令已发送", "当前应用已停止", 1, 3000);
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
                                    text: CutActivityControl.identifier
                                }
                                MLabel {
                                    Layout.preferredWidth: 100
                                    rectColor: colorConstants.suggestClickedColor
                                    text: "前台包名"
                                }
                                Label {
                                    Layout.fillWidth: true
                                    text: CutActivityControl.cutPackageName
                                }
                                MLabel {
                                    Layout.preferredWidth: 100
                                    rectColor: colorConstants.suggestClickedColor
                                    text: "前台活动"
                                }
                                Text {
                                    Layout.fillWidth: true
                                    text: CutActivityControl.cutActivity
                                }
                            }
                        }
                    }
                    MonitorItem {
                        title: "电池电量"
                        minY: 25
                        maxY: 25
                        cutValue: BatteryControl.level
                    }
                    MonitorItem {
                        title: "电池温度"
                        minY: 25
                        maxY: 25
                        cutValue: BatteryControl.temperature
                    }
                    MonitorItem {
                        title: "电池电压"
                        minY: 25
                        maxY: 25
                        cutValue: BatteryControl.voltage
                    }
                    MonitorItem {
                        title: "电池电流"
                        minY: 25
                        maxY: 25
                        cutValue: BatteryControl.current
                    }
                    MonitorItem {
                        title: "内存占用"
                        minY: 25
                        maxY: 25
                        cutValue: RealTimeInfoHelper.memUse
                    }
                }
            }
        }
    }
}
