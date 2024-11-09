import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import MFloat
import "../components"
import FastBootDeviceManager 1.0
import FlashLinkModel 1.0
import FlashTools 1.0
import QtQuick.Dialogs

ItemPage {
    id: root
    ColorConstants {
        id: colorConstants
    }
    ColumnLayout {
        anchors.fill: parent
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

        MWrapper {
            Layout.preferredHeight: 30
            Layout.fillWidth: true
            padding: 0
            RowLayout {
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
                        FastBootDeviceManager.rebootToFastBoot("")
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
                MWrapper {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 130
                    title: "临时启动BOOT镜像"
                    description: "传输boot镜像临时启动，不刷入，常用于twrp刷入的第一步"

                    ColumnLayout {
                        RowLayout {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            MLineEdit {
                                id: bootImageEdit
                                placeholderText: "boot镜像路径"
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                            }
                            MButton {
                                btnType: MButton.FBtnType.Ordinary
                                text: "选择镜像"
                                Layout.preferredWidth: 80
                                Layout.fillHeight: true
                                onClicked: {
                                    startBootImageDialog.open()
                                }
                            }
                        }
                        MButton {
                            text: "临时启动"
                            btnType: MButton.FBtnType.Warning
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            onClicked: {
                                FlashTools.startBoot("", bootImageEdit.editItem.text)
                            }
                        }
                    }
                }
                MWrapper {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 110
                    title: "刷写分区"
                    ColumnLayout {
                        RowLayout {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            MLineEdit {
                                id: flashImageEdit
                                placeholderText: "镜像路径"
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                            }
                            MButton {
                                btnType: MButton.FBtnType.Ordinary
                                text: "选择镜像"
                                Layout.preferredWidth: 80
                                Layout.fillHeight: true
                                onClicked: {
                                    flashImageSelectDialog.open()
                                }
                            }
                        }
                        RowLayout {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            MLineEdit {
                                id: flashPartNameEdit
                                placeholderText: "分区名"
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                            }
                            MButton {
                                btnType: MButton.FBtnType.Warning
                                text: "开始刷写"
                                Layout.preferredWidth: 80
                                Layout.fillHeight: true
                                onClicked: {
                                    FlashTools.flash("", flashPartNameEdit.editItem.text, flashImageEdit.editItem.text)
                                }
                            }
                        }
                    }
                }
                MWrapper {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 110
                    title: "清除分区"
                    ColumnLayout {
                        MLineEdit {
                            id: clearPartEdit
                            placeholderText: "分区名"
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                        }
                        MButton {
                            text: "开始清除"
                            btnType: MButton.FBtnType.Warning
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            onClicked: {
                                FlashTools.clear("", clearPartEdit.editItem.text)
                            }
                        }
                    }
                }
                MWrapper {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    title: "执行刷机脚本"
                    description: "某些线刷包刷写通过脚本实现，可以在这里快捷执行"
                    ColumnLayout {
                        RowLayout {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            MLineEdit {
                                id: exeScriptEdit
                                placeholderText: "刷机脚本路径"
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                            }
                            MButton {
                                btnType: MButton.FBtnType.Ordinary
                                text: "选择路径"
                                Layout.preferredWidth: 80
                                Layout.fillHeight: true
                                onClicked: {
                                    exeScriptPathSelectDialog.open()
                                }
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
                MWrapper {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 130
                    title: "小米快捷刷机"
                    description: "小米的线刷包直接在这里刷写即可"
                    ColumnLayout {
                        RowLayout {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            MLineEdit {
                                id: xiaomiImageEdit
                                placeholderText: "线刷包路径"
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                            }
                            MButton {
                                btnType: MButton.FBtnType.Ordinary
                                text: "选择路径"
                                Layout.preferredWidth: 80
                                Layout.fillHeight: true
                                onClicked: {
                                    xiaomiFlashImageDialog.open()
                                }
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
                MWrapper {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 130
                    title: "快捷解包"
                    description: "解压线刷包，可以提取其中某个分区的镜像"
                    ColumnLayout {
                        RowLayout {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            MLineEdit {
                                id: unzipInEdit
                                placeholderText: "线刷包路径（默认解压payload.img）"
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                            }
                            MButton {
                                btnType: MButton.FBtnType.Ordinary
                                text: "选择路径"
                                Layout.preferredWidth: 80
                                Layout.fillHeight: true
                                onClicked: {
                                    unzipPathInDialog.open()
                                }
                            }
                        }
                        RowLayout {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            MLineEdit {
                                id: unzipOutEdit
                                placeholderText: "输出目录"
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                            }
                            MButton {
                                btnType: MButton.FBtnType.Ordinary
                                text: "选择"
                                Layout.preferredWidth: 40
                                Layout.fillHeight: true
                                onClicked: {
                                    unzipPathOutDialog.open()
                                }
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
                MWrapper {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    title: "友链"
                    description: "收录了部分好用的刷机包下载网站"
                    ScrollView {
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
                                        text: model.title
                                        horizontalAlignment: Text.AlignHCenter
                                        verticalAlignment: Text.AlignVCenter
                                    }

                                    MouseArea {
                                        anchors.fill: parent
                                        onClicked: {
                                            Qt.openUrlExternally(model.url)
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
    FileDialog {
        id: startBootImageDialog
        title: "Select a File"
        fileMode: FileDialog.OpenFile
        onAccepted: {
            bootImageEdit.editItem.text = startBootImageDialog.currentFile
        }
    }
    FileDialog {
        id: flashImageSelectDialog
        title: "Select a File"
        fileMode: FileDialog.OpenFile
        onAccepted: {
            flashImageEdit.editItem.text = flashImageSelectDialog.currentFile
        }
    }
    FileDialog {
        id: exeScriptPathSelectDialog
        title: "Select a File"
        fileMode: FileDialog.OpenFile
        onAccepted: {
            exeScriptEdit.editItem.text = exeScriptPathSelectDialog.currentFile
        }
    }
    FileDialog {
        id: xiaomiFlashImageDialog
        title: "Select a File"
        fileMode: FileDialog.OpenFile
        onAccepted: {
            xiaomiImageEdit.editItem.text = xiaomiFlashImageDialog.currentFile
        }
    }
    FileDialog {
        id: unzipPathInDialog
        title: "Select a File"
        fileMode: FileDialog.OpenFile
        onAccepted: {
            unzipInEdit.editItem.text = unzipPathInDialog.currentFile
        }
    }
    FileDialog {
        id: unzipPathOutDialog
        title: "Select a File"
        fileMode: FileDialog.OpenFile
        onAccepted: {
            unzipOutEdit.editItem.text = unzipPathOutDialog.currentFile
        }
    }
}
