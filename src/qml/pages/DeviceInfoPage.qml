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

    property var monitorItems: [0, 1]

    RowLayout {
        anchors.fill: parent
        MFrame {
            Layout.preferredWidth: parent.width * 0.48
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft
            wrapperColor: Qt.rgba(255, 255, 255, 0.65)
            opacity: 0.9
            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 10
                // 设备Title
                RowLayout {
                    Layout.preferredHeight: 50
                    implicitHeight: 50
                    Layout.fillWidth: true
                    spacing: 20
                    Rectangle {
                        color: "black"
                        Layout.preferredWidth: 60
                        Layout.preferredHeight: 50
                        radius: 10
                        Text {
                            anchors.fill: parent
                            text: DetailInfoControl.info[DetailInfoControl.DETA_MANUFACTURER]
                            color: "white"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                    }

                    ColumnLayout {
                        Text {
                            text: {
                                return DetailInfoControl.info[DetailInfoControl.DETA_DEVICENAME] === "" ?
                                    DetailInfoControl.info[DetailInfoControl.DETA_MODEL]
                                    : DetailInfoControl.info[DetailInfoControl.DETA_DEVICENAME]
                            }
                            font.pixelSize: 18
                            font.bold: true
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        Text {
                            text: "安卓版本: " + DetailInfoControl.info[DetailInfoControl.DETA_ANDROIDVERSION]
                            opacity: 0.8
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                }

                Text {
                    text: "电池信息"
                    font.bold: true
                    font.pixelSize: 18
                    color: colorConstants.ordinaryClickedColor
                }

                BatteryRect {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 50
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

                Text {
                    text: "设备信息"
                    font.bold: true
                    font.pixelSize: 18
                    color: colorConstants.ordinaryClickedColor
                }

                AppInfoItem {
                    title: "硬件"
                    content: {
                        return `设备型号: ${DetailInfoControl.info[DetailInfoControl.DETA_MODEL]}\n设备代号: ${DetailInfoControl.info[DetailInfoControl.DETA_DEVICECODE]}\n主板序列号: ${DetailInfoControl.info[DetailInfoControl.DETA_SERIALNUMBER]}`
                    }
                }

                AppInfoItem {
                    title: "软件"
                    content: {
                        `系统名称: ${DetailInfoControl.info[DetailInfoControl.DETA_SYSTEMINFO]}\n构建版本号: 3847587\n安卓版本: ${DetailInfoControl.info[DetailInfoControl.DETA_ANDROIDVERSION]}`
                    }
                }

                AppInfoItem {
                    title: "CPU"
                    content: {
                        `高通骁龙660\n最大主频 3000MHZ  核心数 8`
                    }
                }

                AppInfoItem {
                    title: "屏幕"
                    content: {
                        `分辨率:  ${DetailInfoControl.info[DetailInfoControl.DETA_RESOLVING]}\nDPI:  ${DetailInfoControl.info[DetailInfoControl.DETA_DPI]}`
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

            MFrame {    // 当前活动信息
                Layout.fillWidth: true
                Layout.preferredHeight: 120
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

            MFrame {
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
                            text: "ADB信息"
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

    Timer {
        interval: 1000
        running: checktepBtn.status
        repeat: true
        onTriggered: {
            var lastX = splineSeries.count - 1
            var lastY = splineSeries.at(lastX).y
            var newX = lastX + 1
            var newY = batteryModel.get(5).info
            if (newY < 10 || newY > 50) {
                return
            }

            splineSeries.append(newX, newY)

            if (lastX > xAxis.max) {
                xAxis.min += 1
                xAxis.max += 1
            }
            if (newY > yAxis.max) {
                yAxis.max = newY
            } else if (newY < yAxis.min) {
                yAxis.min = newY
            }
        }
    }

    Timer {
        id: btyleTi
        interval: 1000
        running: checklevelBtn.status
        repeat: true
        onTriggered: {
            var lastX = splineSeries2.count - 1
            var lastY = splineSeries2.at(lastX).y
            var newX = lastX + 1
            var newY = BatteryControl.level
            if (newY <= 0 || newY > 100) {
                return
            }

            splineSeries2.append(newX, newY)

            if (lastX > xAxis2.max) {
                xAxis2.min += 1
                xAxis2.max += 1
            }
            if (newY > yAxis2.max) {
                yAxis2.max = newY
            } else if (newY < yAxis2.min) {
                yAxis2.min = newY
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
                batteryModel.get(5).info = parseFloat(temperature).toFixed(1)
            });
        }
    }
}
