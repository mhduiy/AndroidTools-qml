import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import MFloat
import "../components"
import FastBootDeviceManager 1.0
import FlashLinkModel 1.0

Item {
    property bool isCurrentPage: false
    id: root
    Layout.minimumWidth: 780
    ColorConstants {
        id: colorConstants
    }
    ColumnLayout {
        anchors.fill: parent
        anchors.rightMargin: 10
        anchors.bottomMargin: 10
        Text {
            Layout.fillWidth: true
            Layout.preferredHeight: 30
            text: "道路千万条，安全第一条，刷机不规范，救砖两行泪"
            color: "red"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 14
            font.bold: true
        }

        MFrame {
            Layout.preferredHeight: 30
            Layout.fillWidth: true
            wrapperColor: Qt.rgba(255, 255, 255, 0.65)
            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 5
                anchors.rightMargin: 5
                Text {
                    text: "请将设备重启到FASTBOOT模式，当前设备: "
                }
                Text {
                    text: FastBootDeviceManager.currentDeviceCode === "" ? "无设备连接" : FastBootDeviceManager.currentDeviceCode
                }

                Item {
                    Layout.fillWidth: true
                }

                MButton {
                    btnType: MButton.FBtnType.Suggest
                    text: "刷新设备"
                    Layout.preferredWidth: 80
                    onClicked: {
                        FastBootDeviceManager.updateDevices()
                        console.log("刷新设备")
                    }
                }
                MButton {
                    btnType: MButton.FBtnType.Warning
                    text: "重启FASTBOOT"
                    Layout.preferredWidth: 80
                    onClicked: {
                        FastBootDeviceManager.rebootToFastBoot(""
                                                               )
                    }
                }
                MButton {
                    btnType: MButton.FBtnType.Warning
                    text: "重启到系统"
                    Layout.preferredWidth: 80
                    onClicked: {
                        FastBootDeviceManager.rebootToSystem("")
                    }
                }
                MButton {
                    btnType: MButton.FBtnType.Warning
                    text: "重启到REC"
                    Layout.preferredWidth: 80
                    onClicked: {
                        FastBootDeviceManager.rebootToRecovery("")
                    }
                }
                MButton {
                    btnType: MButton.FBtnType.Warning
                    text: "关机"
                    Layout.preferredWidth: 80
                    onClicked: {
                        FastBootDeviceManager.powerOff("")
                    }
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            ColumnLayout {
                Layout.fillHeight: true
                Layout.preferredWidth: 350
                Layout.maximumWidth: 350
                StandardComponent {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 120
                    title: "临时启动BOOT镜像"
                    description: "传输boot镜像临时启动，不刷入，常用于twrp刷入的第一步"

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.topMargin: parent.titleSpace + 10
                        anchors.leftMargin: 10
                        anchors.rightMargin: 10
                        anchors.bottomMargin: 10
                        RowLayout {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            MLineEdit {
                                placeholderText: "boot镜像路径"
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                            }
                            MButton {
                                btnType: MButton.FBtnType.Ordinary
                                text: "选择镜像"
                                Layout.preferredWidth: 80
                                Layout.fillHeight: true
                            }
                        }
                        MButton {
                            text: "临时启动"
                            btnType: MButton.FBtnType.Warning
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                        }
                    }
                }
                StandardComponent {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 120
                    title: "刷写分区"
                    description: "选择指定分区进行刷写"
                    ColumnLayout {
                        anchors.fill: parent
                        anchors.topMargin: parent.titleSpace + 10
                        anchors.leftMargin: 10
                        anchors.rightMargin: 10
                        anchors.bottomMargin: 10
                        RowLayout {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            MLineEdit {
                                placeholderText: "镜像路径"
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                            }
                            MButton {
                                btnType: MButton.FBtnType.Ordinary
                                text: "选择镜像"
                                Layout.preferredWidth: 80
                                Layout.fillHeight: true
                            }
                        }
                        RowLayout {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            MLineEdit {
                                placeholderText: "分区名"
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                            }
                            MButton {
                                btnType: MButton.FBtnType.Warning
                                text: "开始刷写"
                                Layout.preferredWidth: 80
                                Layout.fillHeight: true
                            }
                        }
                    }
                }
                StandardComponent {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 120
                    title: "清除分区"
                    description: "指定分区清除全部数据"
                    ColumnLayout {
                        anchors.fill: parent
                        anchors.topMargin: parent.titleSpace + 10
                        anchors.leftMargin: 10
                        anchors.rightMargin: 10
                        anchors.bottomMargin: 10
                        MLineEdit {
                            placeholderText: "分区名"
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                        }
                        MButton {
                            text: "开始清除"
                            btnType: MButton.FBtnType.Warning
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                        }
                    }
                }
                StandardComponent {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    title: "执行刷机脚本"
                    description: "某些线刷包刷写通过脚本实现，可以在这里快捷执行"
                    ColumnLayout {
                        anchors.fill: parent
                        anchors.topMargin: parent.titleSpace + 10
                        anchors.leftMargin: 10
                        anchors.rightMargin: 10
                        anchors.bottomMargin: 10
                        RowLayout {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            MLineEdit {
                                placeholderText: "刷机脚本路径"
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                            }
                            MButton {
                                btnType: MButton.FBtnType.Ordinary
                                text: "选择路径"
                                Layout.preferredWidth: 80
                                Layout.fillHeight: true
                            }
                        }
                        RowLayout {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            MLineEdit {
                                placeholderText: "工作目录"
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                            }
                            MButton {
                                btnType: MButton.FBtnType.Warning
                                text: "开始执行"
                                Layout.preferredWidth: 80
                                Layout.fillHeight: true
                            }
                        }
                    }
                }
            }
            ColumnLayout {
                Layout.fillHeight: true
                Layout.fillWidth: true
                StandardComponent {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 120
                    title: "小米快捷刷机"
                    description: "小米的线刷包直接在这里刷写即可"
                    ColumnLayout {
                        anchors.fill: parent
                        anchors.topMargin: parent.titleSpace + 10
                        anchors.leftMargin: 10
                        anchors.rightMargin: 10
                        anchors.bottomMargin: 10
                        RowLayout {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            MLineEdit {
                                placeholderText: "线刷包路径"
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                            }
                            MButton {
                                btnType: MButton.FBtnType.Ordinary
                                text: "选择路径"
                                Layout.preferredWidth: 80
                                Layout.fillHeight: true
                            }
                        }
                        MButton {
                            text: "开始刷写"
                            btnType: MButton.FBtnType.Warning
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                        }
                    }
                }
                StandardComponent {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 120
                    title: "快捷解包"
                    description: "解压线刷包，可以提取其中某个分区的镜像"
                    ColumnLayout {
                        anchors.fill: parent
                        anchors.topMargin: parent.titleSpace + 10
                        anchors.leftMargin: 10
                        anchors.rightMargin: 10
                        anchors.bottomMargin: 10
                        RowLayout {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            MLineEdit {
                                placeholderText: "线刷包路径（默认解压payload.img）"
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                            }
                            MButton {
                                btnType: MButton.FBtnType.Ordinary
                                text: "选择路径"
                                Layout.preferredWidth: 80
                                Layout.fillHeight: true
                            }
                        }
                        RowLayout {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            MLineEdit {
                                placeholderText: "输出目录"
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                            }
                            MButton {
                                btnType: MButton.FBtnType.Ordinary
                                text: "选择"
                                Layout.preferredWidth: 40
                                Layout.fillHeight: true
                            }
                            MButton {
                                btnType: MButton.FBtnType.Ordinary
                                text: "开始解压"
                                Layout.preferredWidth: 80
                                Layout.fillHeight: true
                            }
                        }
                    }
                }
                StandardComponent {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    title: "友链"
                    description: "收录了部分好用的刷机包下载网站"
                    ScrollView {
                        anchors.fill: parent
                        anchors.topMargin: parent.titleSpace + 10
                        anchors.leftMargin: 10
                        anchors.rightMargin: 10
                        anchors.bottomMargin: 10
                        ScrollBar.vertical: ScrollBar {
                            policy: ScrollBar.AlwaysOff
                        }

                        ListView {
                            spacing: 2
                            clip: true
                            model: FlashLinkModel

                            delegate: Component{
                                Rectangle {
                                    color: "transparent"
                                    width: ListView.view.width
                                    height: 30
                                    border.width: 2
                                    border.color: "gray"
                                    radius: 10
                                    Text {
                                        anchors.fill: parent
                                        text: model.url
                                        horizontalAlignment: Text.AlignHCenter
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
