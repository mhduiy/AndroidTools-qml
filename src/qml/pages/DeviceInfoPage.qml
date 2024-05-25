import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import MFloat

import "../components"
import BatteryControl 1.0
import CutActivityControl 1.0
import ADBControl 1.0
import DetailInfoControl 1.0
import NotificationControl 1.0

Item {
    property bool isCurrentPage: false
    id: root
    Layout.minimumWidth: 780
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

    property var monitorItems: [0, 1]
    ColumnLayout { // 主界面
        anchors.fill: parent
        RowLayout {  //  上方区域
            spacing: 10
            Layout.preferredWidth: parent.width
            Layout.fillWidth: true
            width: parent.width
            Layout.preferredHeight: 250
            Layout.alignment: Qt.AlignTop

            Layout.maximumHeight: 280
            onWidthChanged:  {
                console.log("宽度" + width)
            }
            MFrame {
                Layout.preferredWidth: parent.width * 0.48
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignLeft
                wrapperColor: Qt.rgba(255, 255, 255, 0.65)
                opacity: 0.9
                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 20
                    spacing: 30
                    Rectangle {
                        Layout.preferredHeight: 200
                        Layout.preferredWidth: 120
                        radius: 5 * width / 100
                        color: colorConstants.suggestClickedColor
                        Text {
                            anchors.fill: parent
                            text: "XIAOMI"
                            font.pixelSize: 20
                            color: "white"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                    GridLayout {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.alignment: Qt.AlignVCenter
                        columns: 2
                        Repeater {
                            model: deviceInfoModel
                            MLabel {
                                rectColor: colorConstants.suggestClickedColor
                                text: model.name
                                Layout.row: index
                                Layout.column: 0
                                Layout.preferredWidth: 60
                                Layout.alignment: Qt.AlignLeft
                            }
                        }
                        Repeater {
                            model: deviceInfoModel
                            Label {
                                text: model.info
                                Layout.row: index
                                Layout.fillWidth: true
                                Layout.column: 1
                            }
                        }
                    }
                }
            }

            MFrame {
                Layout.fillWidth: true
                Layout.rightMargin: 10
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignRight
                wrapperColor: Qt.rgba(255, 255, 255, 0.65)

                ColumnLayout {
                    anchors.centerIn: parent
                    ColumnLayout {

                        BatteryRect {
                            level: BatteryControl.level
                        }

                        GridLayout {
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
                                }
                            }
                        }
                    }
                }
            }
        }
        RowLayout { // 下方区域
            spacing: 10
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.bottomMargin: 10

            ColumnLayout {
                Layout.preferredWidth: 300
                Layout.maximumWidth: 300
                Layout.fillHeight: true
                MFrame {    // 当前活动信息
                    id: activityRoot
                    Layout.fillWidth: true
                    Layout.preferredHeight: 150
                    wrapperColor: Qt.rgba(255, 255, 255, 0.65)
                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 20
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
                                    NotificationControl.send("当前应用已停止", 1, 3000);
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
                MFrame {    // 简单设备控制
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    wrapperColor: Qt.rgba(255, 255, 255, 0.65)
                    GridLayout {
                        anchors.centerIn: parent
                        columns: 2
                        MLabel {
                            text: "ADB版本: " + ADBControl.adbVersion
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
                            width: 120
                        }
                        MButton {
                            text: "关闭手机"
                            btnType: MButton.FBtnType.Warning
                            width: 120
                        }
                        MButton {
                            text: "重启到RECOVERY"
                            btnType: MButton.FBtnType.Warning
                            width: 120
                        }
                        MButton {
                            text: "重启到FASTBOOT"
                            btnType: MButton.FBtnType.Warning
                            width: 120
                        }
                    }
                }
            }

            ColumnLayout {  // 监控区域
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.rightMargin: 10

                MFrame {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    wrapperColor: Qt.rgba(255, 255, 255, 0.65)

                    ScrollView {
                        anchors.fill: parent
                        anchors.margins: 10
                        ScrollBar.vertical: ScrollBar {
                            policy: ScrollBar.AlwaysOff
                        }

                        ColumnLayout {
                            width: parent.parent.width
                            Repeater {
                                model: monitorMap.length
                                ColumnLayout {
                                    Layout.fillWidth: true
                                    Layout.preferredWidth: parent.width
                                    RowLayout {
                                        Layout.fillWidth: true
                                        Layout.preferredWidth: parent.width
                                        Text {
                                            text: monitorMap[index].name
                                            font.pixelSize: 14
                                        }

                                        MSwitchButton {
                                            Layout.alignment: Qt.AlignRight
                                        }
                                    }

                                    Rectangle {
                                        Layout.fillWidth: true
                                        height: 100
                                        radius: 5
                                        border.width: 1
                                        color: Qt.rgba(255, 255, 255, 0.65)
                                        border.color: "gray"
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // 模拟数据
    ListModel {
        id: deviceInfoModel
        ListElement { name: "厂商"; info: "-"}
        ListElement { name: "品牌"; info: "-"}
        ListElement { name: "型号"; info: "-"}
        ListElement { name: "设备名"; info: "-"}
        ListElement { name: "设备代号"; info: "-"}
        ListElement { name: "系统信息"; info: "-"}
        ListElement { name: "安卓版本"; info: "-"}
        ListElement { name: "分辨率"; info: "-"}
// 暂时只显示8个数据
//        ListElement { name: "dpi"; info: "-"}
//        ListElement { name: "ip地址"; info: "-"}
//        ListElement { name: "Mac地址"; info: "-"}
//        ListElement { name: "CPU信息"; info: "-"}
//        ListElement { name: "内存容量"; info: "-"}
//        ListElement { name: "SDK版本"; info: "-"}
//        ListElement { name: "序列号"; info: "-"}

        Component.onCompleted: {
            for (var i = 0; i < 8; i++) {
                deviceInfoModel.get(i).info = DetailInfoControl.info[i]
            }
            DetailInfoControl.valueChanged.connect(function() {
                for (var i = 0; i < 8; i++) {
                    deviceInfoModel.get(i).info = DetailInfoControl.info[i]
                }
            });
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
                batteryModel.get(5).info = parseFloat(temperature).toFixed(1) + "℃"
            });
        }
    }
}
