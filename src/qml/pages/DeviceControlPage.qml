import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import MFloat
import "../components"
import DeviceControl 1.0
import BatteryDisguise 1.0
import ResolutionControl 1.0
import FileTransfer 1.0
import InputText 1.0
import StartActivity 1.0

ItemPage {
    id: root
    ColorConstants {
        id: colorConstants
    }

    RowLayout {
        anchors.fill: parent
        ColumnLayout {
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            Layout.preferredWidth: 300
            Layout.maximumWidth: 300
            StandardComponent {
                Layout.preferredHeight: 130
                Layout.fillWidth: true
                title: "音乐控制"
                description: "控制设备音乐状态"
                GridLayout {
                    anchors.fill: parent
                    anchors.topMargin: parent.titleSpace + 10
                    anchors.leftMargin: 10
                    anchors.rightMargin: 10
                    anchors.bottomMargin: 10
                    columns: 4
                    Repeater {
                        model: 4
                        MButton {
                            text: musicControlModel.get(index).name
                            btnType: MButton.FBtnType.Ordinary
                            onClicked: {
                                DeviceControl.control(DeviceControl.CTRL_Music, index)
                            }
                        }
                    }
                    Repeater {
                        model: 2
                        MButton {
                            text: musicControlModel.get(index + 4).name
                            Layout.columnSpan: 2
                            Layout.fillWidth: true
                            btnType: MButton.FBtnType. Suggest
                            onClicked: {
                                DeviceControl.control(DeviceControl.CTRL_Music, index + 4)
                            }
                        }
                    }
                }
            }
            StandardComponent {
                Layout.preferredHeight: 240
                Layout.fillWidth: true
                title: "按键模拟"
                description: "模拟设备按下某个按键，或执行一个操作"
                GridLayout {
                    anchors.fill: parent
                    anchors.topMargin: parent.titleSpace + 10
                    anchors.leftMargin: 10
                    anchors.rightMargin: 10
                    anchors.bottomMargin: 10
                    columns: 3
                    Repeater {
                        model: buttonControlModel.count
                        MButton {
                            text: buttonControlModel.get(index).name
                            btnType: MButton.FBtnType.Ordinary
                            Layout.fillWidth: true
                            onClicked: {
                                DeviceControl.control(DeviceControl.CTRL_Key, index)
                            }
                        }
                    }
                }

            }
            StandardComponent {
                Layout.fillHeight: true
                Layout.fillWidth: true
                title: "广播控制"
                description: "模拟发出一个全局广播，可以模拟设备的某一状态"
                ScrollView {
                    anchors.fill: parent
                    anchors.topMargin: parent.titleSpace + 10
                    anchors.leftMargin: 10
                    anchors.rightMargin: 10
                    anchors.bottomMargin: 10
                    ScrollBar.vertical: ScrollBar {
                        policy: ScrollBar.AlwaysOff
                    }

                    GridLayout {
                        width: parent.parent.width
                        columns: 2
                        Repeater {
                            model: broadcastControlModel.count
                            MButton {
                                text: broadcastControlModel.get(index).name
                                btnType: MButton.FBtnType.Ordinary
                                Layout.fillWidth: true
                                onClicked: {
                                    DeviceControl.control(DeviceControl.CTRL_BoardCast, index)
                                }
                            }
                        }
                    }
                }

            }
        }

        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            ScrollBar.vertical: ScrollBar {
                policy: ScrollBar.AlwaysOff
            }

            ColumnLayout {
                width: parent.parent.width
                StandardComponent {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 170
                    title: "电量伪装"
                    description: "控制手机的充电状态，模拟一个虚假的手机电量"
                    GridLayout {
                        anchors.fill: parent
                        anchors.topMargin: parent.titleSpace + 10
                        anchors.leftMargin: 10
                        anchors.rightMargin: 10
                        anchors.bottomMargin: 10
                        columns: 2
                        MLineEdit {
                            id: batteryLevelEdit
                            editItem.text: "100"
                            placeholderText: "电量"
                            Layout.fillWidth: true
                        }

                        MButton {
                            text: "设置电量"
                            btnType: MButton.FBtnType.Suggest
                            Layout.fillWidth: true
                            onClicked: {
                                var level = parseInt(batteryLevelEdit.editItem.text)
                                if (level >=0 || level <= 100) {
                                    BatteryDisguise.setBatteryLevel(level);
                                }
                            }
                        }
                        MButton {
                            text: "停止充电"
                            btnType: MButton.FBtnType.Ordinary
                            Layout.fillWidth: true
                            onClicked: {
                                BatteryDisguise.stopCharge();
                            }
                        }
                        MButton {
                            text: "恢复充电"
                            btnType: MButton.FBtnType.Ordinary
                            Layout.fillWidth: true
                            onClicked: {
                                BatteryDisguise.restoreCharge();
                            }
                        }
                        MButton {
                            text: "模拟插电不充电"
                            btnType: MButton.FBtnType.Ordinary
                            Layout.fillWidth: true
                            onClicked: {
                                BatteryDisguise.connectButNoCharge();
                            }
                        }
                        MButton {
                            text: "恢复所有状态"
                            btnType: MButton.FBtnType.Ordinary
                            Layout.fillWidth: true
                            onClicked: {
                                BatteryDisguise.restoreAll();
                            }
                        }
                    }

                }

                StandardComponent {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 170
                    title: "分辨率&DPI修改"
                    description: "修改设备的分辨率和显示缩放，可能导致显示异常"
                    ColumnLayout {
                        anchors.fill: parent
                        anchors.topMargin: parent.titleSpace + 10
                        anchors.leftMargin: 10
                        anchors.rightMargin: 10
                        anchors.bottomMargin: 10
                        RowLayout {
                            Layout.fillWidth: true
                            Text {
                                text: "分辨率"
                            }
                            Item {
                                Layout.fillWidth: true
                            }
                            MLineEdit {
                                id: reWidthEdit
                                Layout.preferredWidth: 60
                                placeholderText: "width"
                                editItem.text: ResolutionControl.screenWidth
                            }
                            Text {
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                Layout.preferredWidth: 20
                                text: "x"
                            }
                            MLineEdit {
                                id: reHeightEdit
                                Layout.preferredWidth: 60
                                placeholderText: "height"
                                editItem.text: ResolutionControl.screenHeight
                            }
                        }
                        RowLayout {
                            Layout.fillWidth: true
                            Text {
                                text: "DPI"
                            }
                            Item {
                                Layout.fillWidth: true
                            }
                            MLineEdit {
                                id: dpiEdit
                                Layout.preferredWidth: 60
                                placeholderText: "dpi"
                                editItem.text: ResolutionControl.dpi
                            }
                        }
                        RowLayout {
                            Layout.fillWidth: true
                            MButton {
                                Layout.fillWidth: true
                                text: "恢复"
                                onClicked: {
                                    ResolutionControl.restore()
                                }
                            }
                            MButton {
                                Layout.fillWidth: true
                                btnType: MButton.FBtnType.Suggest
                                text: "设置"
                                onClicked: {
                                    var width = parseInt(reWidthEdit.editItem.text)
                                    var height = parseInt(reHeightEdit.editItem.text)
                                    var dpi = parseInt(dpiEdit.editItem.text)
                                    if (width > 0 && height > 0 && dpi > 0) {
                                        ResolutionControl.set(width, height, dpi)
                                    }
                                }
                            }
                        }
                    }
                }

                StandardComponent {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 120
                    title: "文件传输"
                    description: "支持AndroidTools到设备的文件传输"
                    ColumnLayout {
                        anchors.fill: parent
                        anchors.topMargin: parent.titleSpace + 10
                        anchors.leftMargin: 10
                        anchors.rightMargin: 10
                        anchors.bottomMargin: 10
                        RowLayout {
                            Layout.fillWidth: true
                            MLineEdit {
                                id: sourceFileEdit
                                Layout.fillWidth: true
                                placeholderText: "待传输文件路径"
                            }
                            MButton {
                                Layout.preferredWidth: 60
                                text: "选择文件"
                                onClicked: {
                                    tranSourceFileDialog.open()
                                }
                            }
                        }
                        RowLayout {
                            Layout.fillWidth: true
                            MLineEdit {
                                id: targetDirEdit
                                Layout.fillWidth: true
                                placeholderText: "目标路径"
                                editItem.text: "/sdcard/"
                            }
                            MButton {
                                Layout.preferredWidth: 60
                                btnType: MButton.FBtnType.Suggest
                                text: "开始传输"
                                onClicked: {
                                    FileTransfer.transmission(sourceFileEdit.editItem.text, targetDirEdit.editItem.text)
                                }
                            }
                        }
                    }
                }
                StandardComponent {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 130
                    title: "键盘共享"
                    description: "当设备光标在输入框时，可以在PC上使用键盘控制手机"
                    ColumnLayout {
                        anchors.fill: parent
                        anchors.topMargin: parent.titleSpace + 10
                        anchors.leftMargin: 10
                        anchors.rightMargin: 10
                        anchors.bottomMargin: 10
                        Rectangle {
                            id: textInputEdit
                            Layout.fillWidth: true
                            Layout.preferredHeight: 60
                            radius: 10
                            color: colorConstants.ordinaryClickedColor

                            Behavior on color {
                                ColorAnimation {
                                    duration: 300
                                }
                            }

                            Text {
                                anchors.centerIn: parent
                                color: "white"
                                id: textIndicator
                                font.pixelSize: 14
                                text: "点击这里开始输入"
                            }

                            Keys.onPressed: (event)=> {
                                InputText.pushKey(event.key)
                                event.accepted = true
                            }

                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    textInputEdit.forceActiveFocus()
                                }
                            }

                            onActiveFocusChanged: {
                                if (focus) {
                                    textIndicator.text = "监听键盘中，输入会同步到设备"
                                    textInputEdit.color = colorConstants.suggestClickedColor
                                } else {
                                    textIndicator.text = "点击这里开始输入"
                                    textInputEdit.color = colorConstants.ordinaryClickedColor
                                }
                            }
                        }
                    }
                }
                StandardComponent {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 160
                    title: "启动activity"
                    description: "手动启动某个活动"
                    ColumnLayout {
                        anchors.fill: parent
                        anchors.topMargin: parent.titleSpace + 10
                        anchors.leftMargin: 10
                        anchors.rightMargin: 10
                        anchors.bottomMargin: 10
                        MLineEdit {
                            id: activityNameEdit
                            Layout.fillWidth: true
                            placeholderText: "activity名"
                            editItem.text: "com.tencent.mm/.ui.LauncherUI"
                        }
                        MLineEdit {
                            id: activityArgsEdit
                            Layout.fillWidth: true
                            placeholderText: "启动参数"
                        }
                        MButton {
                            Layout.fillWidth: true
                            btnType: MButton.FBtnType.Suggest
                            text: "启动"
                            onClicked: {
                                StartActivity.start(activityNameEdit.editItem.text, activityArgsEdit.editItem.text)
                            }
                        }
                    }
                }
            }
        }
    }

    ListModel {
        id: musicControlModel
        ListElement { name: "上一曲";}
        ListElement { name: "停止";}
        ListElement { name: "播放/暂停";}
        ListElement { name: "下一曲";}
        ListElement { name: "V -";}
        ListElement { name: "V +";}
    }
    ListModel {
        id: buttonControlModel
        ListElement { name: "菜单";}
        ListElement { name: "HOME";}
        ListElement { name: "返回";}
        ListElement { name: "电源";}
        ListElement { name: "增加亮度";}
        ListElement { name: "减小亮度";}
        ListElement { name: "关机";}
        ListElement { name: "重启";}
        ListElement { name: "重启到RECOVERY";}
        ListElement { name: "重启到FASTBOOT";}
        ListElement { name: "拍照(在相机中)";}
        ListElement { name: "静音";}
        ListElement { name: "打开语音助手";}
        ListElement { name: "光标到行首";}
        ListElement { name: "光标到行尾";}
    }

    ListModel {
        id: broadcastControlModel
        ListElement { name: "网络连接发生变化";}
        ListElement { name: "屏幕点亮";}
        ListElement { name: "屏幕熄灭";}
        ListElement { name: "电量低";}
        ListElement { name: "电量恢复";}
        ListElement { name: "设备启动完毕";}
        ListElement { name: "存储空间过低";}
        ListElement { name: "存储空间恢复";}
        ListElement { name: "安装新的应用";}
        ListElement { name: "WiFi状态变化1";}
        ListElement { name: "WiFi状态变化2";}
        ListElement { name: "电量发生变化";}
        ListElement { name: "输入法发生变化";}
        ListElement { name: "电源连接";}
        ListElement { name: "电源断开";}
        ListElement { name: "系统开始休眠";}
        ListElement { name: "系统停止休眠";}
        ListElement { name: "壁纸发生变化";}
        ListElement { name: "插入耳机";}
        ListElement { name: "卸载外部介质";}
        ListElement { name: "挂载外部介质";}
        ListElement { name: "省电模式开启";}
    }
    FileDialog {
        id: tranSourceFileDialog
        title: "Select a File"
        fileMode: FileDialog.OpenFile
        onAccepted: {
            sourceFileEdit.editItem.text = tranSourceFileDialog.currentFile
        }
    }
}
