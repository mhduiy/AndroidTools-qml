import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import FluentUI
import ConnectManager 1.0
import DeviceControl 1.0
import BatteryDisguise 1.0
import ResolutionControl 1.0
import FileTransfer 1.0
import InputText 1.0
import StartActivity 1.0
import ADT 1.0

FluContentPage {
    id: page
    title: "设备控制"
    property var device: ConnectManager.cutADBDevice

    ScrollView {
        anchors.fill: parent
        contentWidth: availableWidth

        ColumnLayout {
            width: parent.width
            spacing: 12

            FluFrame {
                Layout.fillWidth: true
                Layout.preferredHeight: 80
                Layout.leftMargin: 20
                Layout.rightMargin: 20
                Layout.topMargin: 12
                visible: !device
                FluText {
                    anchors.centerIn: parent
                    text: "连接设备以使用控制功能"
                    color: FluTheme.fontSecondaryColor
                }
            }

            FluFrame {
                Layout.fillWidth: true
                Layout.leftMargin: 20
                Layout.rightMargin: 20
                visible: !!device
                ColumnLayout {
                    anchors { left: parent.left; top: parent.top; right: parent.right; margins: 16 }
                    spacing: 8
                    FluText { text: "媒体控制"; font: FluTextStyle.Subtitle }
                    GridLayout {
                        columns: 3
                        columnSpacing: 6
                        rowSpacing: 6
                        Layout.fillWidth: true
                        Repeater {
                            model: ["上一曲","停止","播放","下一曲","音量-","音量+"]
                            FluButton {
                                text: modelData
                                Layout.fillWidth: true
                                onClicked: DeviceControl.control(ADT.Music, index)
                            }
                        }
                    }
                }
            }

            FluFrame {
                Layout.fillWidth: true
                Layout.leftMargin: 20
                Layout.rightMargin: 20
                visible: !!device
                ColumnLayout {
                    anchors { left: parent.left; top: parent.top; right: parent.right; margins: 16 }
                    spacing: 8
                    FluText { text: "按键模拟"; font: FluTextStyle.Subtitle }
                    GridLayout {
                        columns: 4
                        columnSpacing: 6
                        rowSpacing: 6
                        Layout.fillWidth: true
                        Repeater {
                            model: ["菜单","主页","返回","电源","亮度+","亮度-","关机","重启","恢复","引导","截图","静音","助手","主页↑","结束↓"]
                            FluButton {
                                text: modelData
                                Layout.fillWidth: true
                                onClicked: DeviceControl.control(ADT.Key, index)
                            }
                        }
                    }
                }
            }

            FluExpander {
                Layout.fillWidth: true
                Layout.leftMargin: 20
                Layout.rightMargin: 20
                visible: !!device
                headerText: "广播模拟"
                content: GridLayout {
                    columns: 3
                    columnSpacing: 6
                    rowSpacing: 6
                    Repeater {
                        model: ["网络状态","亮屏","息屏","电量低","电量正常","启动完成","存储低","存储正常","安装","WiFi1","WiFi2","充电状态","输入法","上电","断电","休眠开","休眠关","壁纸","耳机","卸载","挂载","省电"]
                        FluButton {
                            text: modelData
                            Layout.fillWidth: true
                            onClicked: DeviceControl.control(ADT.Broadcast, index)
                        }
                    }
                }
            }

            FluExpander {
                Layout.fillWidth: true
                Layout.leftMargin: 20
                Layout.rightMargin: 20
                visible: !!device
                headerText: "电池伪装"
                content: ColumnLayout {
                    spacing: 8
                    RowLayout {
                        FluTextBox { id: bl; placeholderText: "电量"; text: "100"; Layout.preferredWidth: 100 }
                        FluButton { text: "设置"; onClicked: BatteryDisguise.setBatteryLevel(parseInt(bl.text) || 100) }
                    }
                    RowLayout {
                        FluButton { text: "停止充电"; Layout.fillWidth: true; onClicked: BatteryDisguise.stopCharge() }
                        FluButton { text: "恢复"; Layout.fillWidth: true; onClicked: BatteryDisguise.restoreCharge() }
                        FluButton { text: "USB不充电"; Layout.fillWidth: true; onClicked: BatteryDisguise.connectButNoCharge() }
                        FluButton { text: "全部重置"; Layout.fillWidth: true; onClicked: BatteryDisguise.restoreAll() }
                    }
                }
            }

            FluExpander {
                Layout.fillWidth: true
                Layout.leftMargin: 20
                Layout.rightMargin: 20
                visible: !!device
                headerText: "分辨率/DPI"
                content: GridLayout {
                    columns: 3
                    columnSpacing: 8
                    rowSpacing: 8
                    FluTextBox { id: we; placeholderText: "宽度"; text: ResolutionControl.screenWidth; Layout.fillWidth: true }
                    FluTextBox { id: he; placeholderText: "高度"; text: ResolutionControl.screenHeight; Layout.fillWidth: true }
                    FluTextBox { id: de; placeholderText: "DPI"; text: ResolutionControl.dpi; Layout.fillWidth: true }
                    FluButton { text: "恢复"; Layout.fillWidth: true; onClicked: ResolutionControl.restore() }
                    FluButton {
                        text: "应用"
                        Layout.fillWidth: true
                        Layout.columnSpan: 2
                        onClicked: {
                            var w = parseInt(we.text)
                            var h = parseInt(he.text)
                            var d = parseInt(de.text)
                            if (w && h && d) ResolutionControl.set(w, h, d)
                        }
                    }
                }
            }

            FluExpander {
                Layout.fillWidth: true
                Layout.leftMargin: 20
                Layout.rightMargin: 20
                visible: !!device
                headerText: "文件传输"
                content: ColumnLayout {
                    spacing: 8
                    RowLayout {
                        FluTextBox { id: fs; placeholderText: "源路径"; Layout.fillWidth: true }
                        FluButton { text: "浏览"; onClicked: fd.open() }
                    }
                    RowLayout {
                        FluTextBox { id: fdst; placeholderText: "目标路径"; text: "/sdcard/"; Layout.fillWidth: true }
                        FluButton { text: "传输"; onClicked: FileTransfer.transmission(fs.text, fdst.text) }
                    }
                }
            }

            FluExpander {
                Layout.fillWidth: true
                Layout.leftMargin: 20
                Layout.rightMargin: 20
                visible: !!device
                headerText: "键盘共享"
                content: Rectangle {
                    implicitWidth: 200
                    implicitHeight: 56
                    radius: 8
                    color: FluTheme.dark ? Qt.rgba(1,1,1,0.04) : Qt.rgba(0,0,0,0.03)
                    border.color: kb.activeFocus ? FluTheme.primaryColor : Qt.rgba(0,0,0,0.1)
                    border.width: 1
                    FluText {
                        anchors.centerIn: parent
                        text: kb.activeFocus ? "监听中..." : "点击此处向设备输入"
                        color: FluTheme.fontSecondaryColor
                    }
                    MouseArea {
                        id: kb
                        anchors.fill: parent
                        focus: true
                        activeFocusOnTab: true
                        Keys.onPressed: function(e) { InputText.pushKey(e.key); e.accepted = true }
                    }
                }
            }

            FluExpander {
                Layout.fillWidth: true
                Layout.leftMargin: 20
                Layout.rightMargin: 20
                visible: !!device
                headerText: "启动活动"
                content: ColumnLayout {
                    spacing: 8
                    FluTextBox { id: an; placeholderText: "Activity名称"; text: "com.tencent.mm/.ui.LauncherUI"; Layout.fillWidth: true }
                    FluTextBox { id: aa; placeholderText: "参数"; Layout.fillWidth: true }
                    FluButton { text: "启动"; Layout.fillWidth: true; onClicked: StartActivity.start(an.text, aa.text) }
                }
            }
        }
    }

    FileDialog { id: fd; title: "选择文件"; fileMode: FileDialog.OpenFile; onAccepted: fs.text = String(currentFile) }
}
