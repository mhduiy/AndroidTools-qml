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
import QtCharts 2.7

ItemPage {
    id: root
    state: "pageShow"
    property var monitorMap: [
        {"name": "CPU占用"},
        {"name": "CPU温度"},
        {"name": "GPU占用"},
        {"name": "电池温度"},
        {"name": "电池电压"},
        {"name": "电池电流"},
        {"name": "电池功率"},
        {"name": "FPS"},
    ]

    ColorConstants {
        id: colorConstants
    }

    function gInfo(key) {
        console.warn("----", DetailInfoControl.info[key])
        return DetailInfoControl.info[key]
    }

    property var monitorItems: [0, 1]

    RowLayout {
        anchors.fill: parent
        MWrapper {
            Layout.preferredWidth: parent.width * 0.48
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft
            ScrollView {
                id: scrollView
                ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
                ScrollBar.vertical.policy: ScrollBar.AlwaysOff
                ColumnLayout {
                   width: scrollView.availableWidth
                    // 设备Title
                    spacing: 10
                    Text {
                        text: {
                            let title = DetailInfoControl.info[DetailInfoControl.DETA_MANUFACTURER];
                            if (title === "" || title === undefined) {
                                title = "请连接设备"
                                return title;
                            }
                            title = title + " "
                                    + (DetailInfoControl.info[DetailInfoControl.DETA_DEVICENAME] === "" ?
                                        DetailInfoControl.info[DetailInfoControl.DETA_MODEL]
                                      : DetailInfoControl.info[DetailInfoControl.DETA_DEVICENAME])
                            return title;
                        }
                        font.pixelSize: 22
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                    Text {
                        text: {
                            let str = DetailInfoControl.info[DetailInfoControl.DETA_SERIALNUMBER]
                            if (str === "" || str === undefined) {
                                str = "请连接设备"
                            }
                            return "设备序列号:" + str;
                        }
                        opacity: 0.8
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                    Item {
                        Layout.preferredHeight: 10
                    }

                    Repeater {
                        id: deviceDetailInfoRep
                        property var texts: [
                            ["厂商", "品牌", "型号", "设备名", "设备代号"],
                            ["CPU信息", "最大频率", "核心数"],
                            ["系统", "安卓版本", "sdk版本"],
                            ["尺寸", "分辨率", "DPI"],
                            ["MAC地址", "IP地址", "内存容量"]
                        ]
                        property var values: [
                            [ DetailInfoControl.info[DetailInfoControl.DETA_MANUFACTURER],  DetailInfoControl.info[DetailInfoControl.DETA_BRAND],  DetailInfoControl.info[DetailInfoControl.DETA_MODEL],  DetailInfoControl.info[DetailInfoControl.DETA_DEVICENAME],  DetailInfoControl.info[DetailInfoControl.DETA_DEVICECODE]],
                            [ DetailInfoControl.info[DetailInfoControl.DETA_CPUINFO],  DetailInfoControl.info[DetailInfoControl.DATE_MAXFREP],  DetailInfoControl.info[DetailInfoControl.DATE_MAXCORENUM]],
                            ["未知",  DetailInfoControl.info[DetailInfoControl.DETA_ANDROIDVERSION],  DetailInfoControl.info[DetailInfoControl.DETA_SDKVERSION]],
                            ["未知",  DetailInfoControl.info[DetailInfoControl.DETA_RESOLVING],  DetailInfoControl.info[DetailInfoControl.DETA_DPI]],
                            [ DetailInfoControl.info[DetailInfoControl.DETA_MACADDR],  DetailInfoControl.info[DetailInfoControl.DETA_IPADDR],  DetailInfoControl.info[DetailInfoControl.DETA_MEMORY]]
                        ]
                        property var titles: ["设备信息", "处理器", "系统信息", "屏幕", "其他信息"]
                        model: texts.length
                        ColumnLayout {
                            Text {
                                text: deviceDetailInfoRep.titles[index]
                                font.bold: true
                                font.pixelSize: 18
                                color: colorConstants.ordinaryClickedColor
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
                                    }

                                    Text {
                                        text: deviceDetailInfoRep.texts[rep.repIndex][index]
                                        opacity: 0.4
                                        font.pixelSize: 14
                                    }

                                    Rectangle {
                                        Layout.fillWidth: true
                                        Layout.preferredHeight: 1
                                        color: Qt.rgba(255, 255, 255, 0.7)
                                    }

                                    Text {
                                        visible: deviceDetailInfoRep.values[rep.repIndex][index] !== undefined
                                        text: deviceDetailInfoRep.values[rep.repIndex][index]
                                    }

                                    MLoadIndicator {
                                        scale: 0.8
                                        visible: deviceDetailInfoRep.values[rep.repIndex][index] === undefined
                                        Component.onCompleted: {
                                            start()
                                        }
                                    }
                                }
                            }
                        }

                    }

                    Item {
                        Layout.fillHeight:  true
                    }
                }
            }


        }
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignTop

            MFrame {
                Layout.fillWidth: true
                Layout.preferredHeight: 120
                wrapperColor: Qt.rgba(255, 255, 255, 0.65)
                GridLayout {
                    anchors.fill: parent
                    anchors.margins: 10
                    columns: 2
                    Text {
                        font.bold: true
                        font.family: "黑体"
                        font.pixelSize: 20
                        text: "快速控制"
                    }
                    MButton {
                        text: "重启ADB"
                        btnType: MButton.FBtnType.Warning
                        Layout.alignment: Qt.AlignRight
                        onClicked: {
                            ADBControl.restartADB();
                        }
                    }
                    MButton {
                        text: "重启手机"
                        btnType: MButton.FBtnType.Warning
                        Layout.fillWidth: true
                        onClicked: {
                            DeviceControl.control(DeviceControl.CTRL_Key, DeviceControl.Reboot)
                        }
                    }
                    MButton {
                        text: "关闭手机"
                        btnType: MButton.FBtnType.Warning
                        Layout.fillWidth: true
                        onClicked: {
                            DeviceControl.control(DeviceControl.CTRL_Key, DeviceControl.Poweroff)
                        }
                    }
                    MButton {
                        text: "重启到RECOVERY"
                        btnType: MButton.FBtnType.Warning
                        Layout.fillWidth: true
                        onClicked: {
                            DeviceControl.control(DeviceControl.CTRL_Key, DeviceControl.RebootToRec)
                        }
                    }
                    MButton {
                        text: "重启到FASTBOOT"
                        btnType: MButton.FBtnType.Warning
                        Layout.fillWidth: true
                        onClicked: {
                            DeviceControl.control(DeviceControl.CTRL_Key, DeviceControl.RebootToFB)
                        }
                    }
                }
            }

            MWrapper {
                Layout.fillWidth: true
                Layout.preferredHeight: 160
                title: "电池信息"
                ColumnLayout {
                    BatteryRect {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 40
                        level: BatteryControl.level
                    }

                    GridLayout {
                        Layout.fillWidth: true
                        columns: 4
                        Repeater {
                            model: batteryModel
                            MLabel {
                                rectColor: colorConstants.suggestClickedColor
                                text: model.name
                                Layout.row: index / 2
                                Layout.column: index % 2 == 1 ? 2 : 0
                            }
                        }
                        Repeater {
                            model: batteryModel
                            Label {
                                text: model.info
                                Layout.row: index / 2
                                Layout.column: index % 2 == 1 ? 3 : 1
                                Layout.fillWidth: true
                            }
                        }
                    }
                }
            }

            MFrame {
                Layout.fillWidth: true
                Layout.preferredHeight: 140
                wrapperColor: Qt.rgba(255, 255, 255, 0.65)
                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 10
                    Layout.alignment: Qt.AlignTop
                    MonitorBarItem {
                        title: "CPU温度"
                        valueStr: "46"
                        value: 20
                    }
                    MonitorBarItem {
                        title: "CPU占用"
                        valueStr: "46"
                        value: 40
                    }
                    MonitorBarItem {
                        title: "内存占用"
                        valueStr: "46"
                        value: 35
                    }
                    MonitorBarItem {
                        title: "磁盘占用"
                        valueStr: "46"
                        value: 80
                    }
                    MonitorBarItem {
                        title: "电池温度"
                        valueStr: "46"
                        value: 60
                    }
                }
            }

            MFrame {    // 当前活动信息
                Layout.fillWidth: true
                Layout.fillHeight: true
                wrapperColor: Qt.rgba(255, 255, 255, 0.65)
                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 10
                    RowLayout {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 30
                        Text {
                            font.bold: true
                            font.family: "黑体"
                            font.pixelSize: 20
                            text: "当前前台应用"
                        }
                        Item {
                            Layout.fillWidth: true
                        }

                        MButton {
                            Layout.preferredWidth: 80
                            text: "停止当前应用"
                            btnType: MButton.FBtnType.Warning
                            onClicked: {
                                CutActivityControl.killCutActivity()
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
                        Text {
                            Layout.fillWidth: true
                            text: CutActivityControl.identifier
                        }
                        MLabel {
                            Layout.preferredWidth: 100
                            rectColor: colorConstants.suggestClickedColor
                            text: "前台包名"
                        }
                        Text {
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
        }
    }


    ListModel {
        id: batteryModel

        ListElement {
            name: "充电模式"
            info: "-"
        }
        ListElement {
            name: "健康度"
            info: "-"
        }
        ListElement {
            name: "电压"
            info: "-"
        }
        ListElement {
            name: "电流"
            info: "-"
        }
        ListElement {
            name: "功率"
            info: "-"
        }
        ListElement {
            name: "温度"
            info: "-"
        }

        Component.onCompleted: {
            BatteryControl.chargeModeChanged.connect(function(mode) {
                var str = "正在放电"
                if (mode === 0) {
                    str = "常规充电"
                } else if (mode === 1) {
                    str = "USB充电"
                } else if (mode === 2) {
                    str = "无线充电"
                } else if (mode === 3) {
                    str = "Dock充电"
                }
                batteryModel.get(0).info = str
            });
            BatteryControl.healthChanged.connect(function(health) {
                batteryModel.get(1).info = health + "%"
            });
            BatteryControl.voltageChanged.connect(function(voltage) {
                batteryModel.get(2).info = voltage + "mV"
            });
            BatteryControl.currentChanged.connect(function(current) {
                batteryModel.get(3).info = current + "mA"
            });
            BatteryControl.powerChanged.connect(function(power) {
                batteryModel.get(4).info = power + "w"
            });
            BatteryControl.temperatureChanged.connect(function(temperature) {
                batteryModel.get(5).info = parseFloat(temperature).toFixed(1)
            });
        }
    }
}
