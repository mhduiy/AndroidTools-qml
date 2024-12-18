import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import MFloat
import SoftListModel 1.0
import AppDetailControl 1.0

ItemPage {
    id: root
    ColorConstants {
        id: colorConstants
    }
    RowLayout {
        anchors.fill: parent

        MWrapper {
            Layout.minimumWidth: 300
            Layout.preferredWidth: parent.width * 0.4
            Layout.fillHeight: true
            title: "软件列表"
            titleRightContent: ComboBox {
                id: softListComboBox
                Layout.minimumWidth: 80
                model: [
                    "第三方应用",
                    "系统应用",
                    "所有应用"
                ]
            }

            ColumnLayout {
                ListView {
                    id: softListView
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    model: SoftListModel
                    spacing: 2
                    clip: true
                    focus: true
                    delegate: softListDelegate
                }
            }
        }
        Component{
            id: softListDelegate
            Rectangle {
                id: wrapper
                width: ListView.view.width
                height: 40
                border.width: 1
                color: {
                    if (ListView.isCurrentItem) {
                        return Qt.rgba(0, 0, 0, 0.3);
                    } else if (listMouseArea.containsMouse) {
                        return Qt.rgba(0, 0, 0, 0.15);
                    } else {
                        return "transparent"
                    }
                }
                border.color: {
                    if (model.state === 1) {
                        return "red"
                    } else {
                        return "gray"
                    }
                }

                radius: 6

                Behavior on color {
                    ColorAnimation {
                        duration: 200
                    }
                }

                Image {
                    id: softListImage
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 5
                    width: 30
                    height: 30
                    source: "qrc:/res/successIcon.png"
                    sourceSize: Qt.size(2 * width, 2 * height)
                    asynchronous: true
                }

                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 40
                    text: model.packageName
                    font.pixelSize: 14
                }

                ColumnLayout {
                    anchors.right: parent.right
                    anchors.rightMargin: 5
                    anchors.verticalCenter: parent.verticalCenter
                    height: parent.height - 4
                    spacing: 1
                    MLabel {
                        rectColor: colorConstants.suggestClickedColor
                        vMargin: 1
                        hMargin: 4
                        text: {
                            if (model.state === 0) {
                                return "启用"
                            } else if (model.state === 1) {
                                return "禁用"
                            } else {
                                return "未知"
                            }
                        }
                        Layout.alignment: Qt.AlignRight
                    }
                    MLabel {
                        rectColor: colorConstants.warningClickedColor
                        hMargin: 4
                        vMargin: 1
                        text: model.versionCode
                        Layout.alignment: Qt.AlignRight
                    }
                }
                MouseArea {
                    id: listMouseArea
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {
//                        softListView.currentIndex = index
                        AppDetailControl.updateInfo(model.packageName)
                    }
                }
            }
        }

        ColumnLayout {
            Layout.fillHeight: true
            Layout.preferredWidth: parent.width * 0.5
            MWrapper {
                Layout.fillWidth: true
                Layout.preferredHeight: parent.height * 0.5
                title: "包名"
                titleRightContent: Text {
                    text: AppDetailControl.packageName
                    font.pixelSize: 16
                    Layout.columnSpan: 5
                    Layout.fillWidth: true
                }

                GridLayout {
                    columns: 6
                    Repeater {
                        model: softDetailInfoModel
                        MLabel {
                            text: model.name
                            Layout.row: index / 3 + 1
                            Layout.column: (index % 3) * 2
                            Layout.preferredWidth: 60
                            rectColor: colorConstants.suggestClickedColor
                            vMargin: 4
                            hMargin: 4
                        }
                    }

                    Repeater {
                        model: softDetailInfoModel
                        Text {
                            text: model.info
                            Layout.row: index / 3 + 1
                            Layout.column: (index % 3) * 2 + 1
                            Layout.fillWidth: true
                        }
                    }

                    MButton {
                        text: "提取软件"
                        btnType: MButton.FBtnType.Ordinary
                        Layout.columnSpan: 2
                        Layout.fillWidth: true
                        onClicked: {
                            AppDetailControl.extractApp(AppDetailControl.packageName, "")
                        }
                    }

                    MButton {
                        text: "冻结软件"
                        btnType: MButton.FBtnType.Ordinary
                        Layout.columnSpan: 2
                        Layout.fillWidth: true
                        onClicked: {
                            AppDetailControl.freezeApp(AppDetailControl.packageName)
                        }
                    }

                    MButton {
                        text: "解冻软件"
                        btnType: MButton.FBtnType.Ordinary
                        Layout.columnSpan: 2
                        Layout.fillWidth: true
                        onClicked: {
                            AppDetailControl.unfreezeApp(AppDetailControl.packageName)
                        }
                    }

                    MButton {
                        text: "卸载软件"
                        btnType: MButton.FBtnType.Warning
                        Layout.columnSpan: 2
                        Layout.fillWidth: true
                        onClicked: {
                            AppDetailControl.uninstallApp(AppDetailControl.packageName)
                        }
                    }

                    MButton {
                        text: "清除数据"
                        btnType: MButton.FBtnType.Warning
                        Layout.columnSpan: 2
                        Layout.fillWidth: true
                        onClicked: {
                            AppDetailControl.clearData(AppDetailControl.packageName)
                        }
                    }

                    MButton {
                        text: "强行停止"
                        btnType: MButton.FBtnType.Warning
                        Layout.columnSpan: 2
                        Layout.fillWidth: true
                        onClicked: {
                            AppDetailControl.stopApp(AppDetailControl.packageName)
                        }
                    }

                    MLineEdit {
                        Layout.columnSpan: 2
                        placeholderText: "启动activty"
                        Layout.fillWidth: true
                    }
                    MLineEdit {
                        Layout.columnSpan: 2
                        placeholderText: "启动参数"
                        Layout.fillWidth: true
                    }
                    MButton{
                        text: "启动"
                        btnType: MButton.FBtnType.Suggest
                        Layout.fillWidth: true
                        Layout.preferredWidth: 40
                        Layout.columnSpan: 2
                        onClicked: {
                            AppDetailControl.startApp(AppDetailControl.packageName)
                        }
                    }
                }
            }

            MWrapper {
                Layout.fillWidth: true
                Layout.fillHeight: true
                title: "安装软件到设备"

                ColumnLayout {
                    ColumnLayout {
                        Layout.fillWidth: true
                        Repeater {
                            model: installSoftOptionModel
                            RowLayout {
                                Layout.fillWidth: true
                                Layout.preferredWidth: 30
                                spacing: 10
                                Text {
                                    text: model.name
                                    font.pixelSize: 12
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                }
                                Item {
                                    Layout.fillWidth: true
                                }
                                MSwitchButton {
                                    Layout.preferredWidth: 50
                                }
                            }
                        }
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        MLineEdit {
                            id: appPackagePathEdit
                            Layout.fillWidth: true
                            placeholderText: "安装包路径"
                        }
                        MButton {
                            text: "选择路径"
                            btnType: MButton.FBtnType.Ordinary
                            onClicked: {
                                installAppSelectAppDialog.open()
                            }
                        }
                    }

                    MButton {
                        Layout.fillWidth: true
                        text: "开始安装"
                        btnType: MButton.FBtnType.Suggest
                        onClicked: {
                            AppDetailControl.installApp(appPackagePathEdit.editItem.text)
                        }
                    }
                }
            }
        }
    }

    ListModel {
        id: softDetailInfoModel

        ListElement {
            name: "版本号:"
            info: "-"
        }
        ListElement {
            name: "安装时间:"
            info: "-"
        }
        ListElement {
            name: "安装者:"
            info: "-"
        }
        ListElement {
            name: "目标SDK:"
            info: "-"
        }
        ListElement {
            name: "最小SDK:"
            info: "-"
        }
        ListElement {
            name: "APPID"
            info: "-"
        }

        Component.onCompleted: {
            softDetailInfoModel.get(0).info = AppDetailControl.versionCode
            softDetailInfoModel.get(1).info = AppDetailControl.installDate
            softDetailInfoModel.get(2).info = AppDetailControl.installUser
            softDetailInfoModel.get(3).info = AppDetailControl.targetSdk
            softDetailInfoModel.get(4).info = AppDetailControl.minSdk
            softDetailInfoModel.get(5).info = AppDetailControl.appId
            AppDetailControl.valueChanged.connect(function() {
                console.log("88888888888", AppDetailControl.appId)
                softDetailInfoModel.get(0).info = AppDetailControl.versionCode
                softDetailInfoModel.get(1).info = AppDetailControl.installDate
                softDetailInfoModel.get(2).info = AppDetailControl.installUser
                softDetailInfoModel.get(3).info = AppDetailControl.targetSdk
                softDetailInfoModel.get(4).info = AppDetailControl.minSdk
                softDetailInfoModel.get(5).info = AppDetailControl.appId
            });
        }
    }
    ListModel {
        id: softDetailBtnModel
        ListElement { name: "提取软件" }
        ListElement { name: "冻结软件" }
        ListElement { name: "解冻软件" }
        ListElement { name: "卸载软件" }
        ListElement { name: "清除数据" }
        ListElement { name: "强行停止" }
    }

    ListModel {
        id: installSoftOptionModel
        ListElement { name: "允许覆盖安装" }
        ListElement { name: "安装到sdcard" }
        ListElement { name: "允许降级安装" }
        ListElement { name: "授予所有运行时权限" }
    }
    FileDialog {
        id: installAppSelectAppDialog
        title: "Select a File"
        fileMode: FileDialog.OpenFile
        onAccepted: {
            appPackagePathEdit.editItem.text = installAppSelectAppDialog.currentFile
        }
    }
}
