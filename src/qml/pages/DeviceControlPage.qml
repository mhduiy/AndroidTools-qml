import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import MFloat
import "../components"

Item {
    id: root
    Layout.minimumWidth: 780

    RowLayout {
        anchors.fill: parent
        anchors.bottomMargin: 10
        ColumnLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            StandardComponent {
                height: 130
                width: 300
                title: "音乐控制"
                description: "控制设备音乐状态"
                contentItem:  Component {
                    GridLayout {
                        columns: 4
                        Repeater {
                            model: 4
                            MButton {
                                text: musicControlModel.get(index).name
                                btnType: MButton.FBtnType.Ordinary
                            }
                        }
                        Repeater {
                            model: 2
                            // 我想每一个model占表格的两格，怎么做
                            MButton {
                                text: musicControlModel.get(index + 4).name
                                Layout.columnSpan: 2
                                Layout.fillWidth: true
                                btnType: MButton.FBtnType. Suggest
                            }
                        }
                    }
                }
            }
            StandardComponent {
                height: 240
                width: 300
                title: "按键模拟"
                description: "模拟设备按下某个按键，或执行一个操作"
                contentItem:  Component {
                    GridLayout {
                        Layout.preferredWidth: parent.parent.parent.width
                        Layout.preferredHeight: parent.parent.parent.height

                        columns: 3
                        Repeater {
                            model: buttonControlModel.count
                            MButton {
                                text: buttonControlModel.get(index).name
                                btnType: MButton.FBtnType.Ordinary
                                width: parent.Layout.preferredWidth / 3 - 5
                            }
                        }
                    }
                }
            }
            StandardComponent {
                Layout.fillHeight: true
                width: 300
                title: "广播控制"
                description: "模拟发出一个全局广播，可以模拟设备的某一状态"
                contentItem:  Component {

                    ScrollView {
                        width: 300
                        height: parent.parent.height
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
                                    width: 130
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
            Layout.rightMargin: 10
            ScrollBar.vertical: ScrollBar {
                policy: ScrollBar.AlwaysOff
            }

            ColumnLayout {
                Layout.preferredWidth: parent.width
                width: parent.width
                StandardComponent {
                    width: parent.width
                    height: 170

                    title: "电量伪装"
                    description: "控制手机的充电状态，模拟一个虚假的手机电量"

                    contentItem: Component {
                        GridLayout {
                            columns: 2
                            width: parent.parent.width
                            TextEdit {
                                Layout.fillWidth: true
                                Layout.maximumWidth: parent.width / 2
                            }

                            MButton {
                                text: "设置电量"
                                btnType: MButton.FBtnType.Suggest
                                Layout.fillWidth: true
                            }
                            MButton {
                                text: "停止充电"
                                btnType: MButton.FBtnType.Ordinary
                                Layout.fillWidth: true
                            }
                            MButton {
                                text: "恢复充电"
                                btnType: MButton.FBtnType.Ordinary
                                Layout.fillWidth: true
                            }
                            MButton {
                                text: "模拟插电不充电"
                                btnType: MButton.FBtnType.Ordinary
                                Layout.fillWidth: true
                            }
                            MButton {
                                text: "恢复所有状态"
                                btnType: MButton.FBtnType.Ordinary
                                Layout.fillWidth: true
                            }
                        }
                    }
                }
                Repeater {
                    model: 20
                    StandardComponent {
                        width: parent.width
                        height: 200
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
}
