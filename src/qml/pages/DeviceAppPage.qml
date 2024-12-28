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
                model: ["第三方应用", "系统应用", "所有应用"]
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
        Component {
            id: softListDelegate
            Rectangle {
                id: wrapper
                width: ListView.view.width
                height: 50
                border.width: 1
                color: {
                    if (model.packageName === AppDetailControl.packageName) {
                        return Qt.rgba(0, 0, 0, 0.3);
                    } else if (listMouseArea.containsMouse) {
                        return Qt.rgba(0, 0, 0, 0.15);
                    } else {
                        return "transparent";
                    }
                }
                border.color: {
                    if (model.state === 1) {
                        return "red";
                    } else {
                        return "gray";
                    }
                }

                radius: 6

                Behavior on color {
                    ColorAnimation {
                        duration: 200
                    }
                }

                Rectangle {
                    id: softListImage
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 5
                    width: 30
                    height: 30
                    radius: width / 2
                    color: "#008c8c"
                    Text {
                        anchors.fill: parent
                        text: model.packageName[0].toUpperCase()
                        color: "white"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: 14
                    }
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
                                return "启用";
                            } else if (model.state === 1) {
                                return "禁用";
                            } else {
                                return "未知";
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
                        AppDetailControl.updateInfo(model.packageName);
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

                ColumnLayout {
                    RowLayout {
                        Layout.preferredHeight: 120
                        Layout.fillWidth: true
                        spacing: 20
                        Rectangle {
                            Layout.preferredWidth: 110
                            Layout.preferredHeight: 110
                            color: "black"
                            radius: 18
                            opacity: 0.8
                            Text {
                                anchors.fill: parent
                                text: AppDetailControl.packageName[0].toUpperCase()
                                color: "white"
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                font.pixelSize: 38
                            }
                        }

                        ColumnLayout {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            spacing: 5
                            Text {
                                text: AppDetailControl.packageName === "" ? "请选择应用" : AppDetailControl.packageName
                                font.pixelSize: 16
                                horizontalAlignment: Text.AlignRight
                            }
                            Repeater {
                                model: softDetailInfoModel
                                RowLayout {
                                    Text {
                                        text: model.name
                                        font.pixelSize: 12
                                        horizontalAlignment: Text.AlignRight
                                        color: "gray"
                                    }
                                    Text {
                                        text: model.info
                                        font.pixelSize: 12
                                        color: "gray"
                                    }
                                }
                            }
                        }
                    }

                    GridLayout {
                        columns: 6
                        rowSpacing: 10
                        columnSpacing: 20
                        uniformCellHeights: true
                        uniformCellWidths: true
                        MButton {
                            text: "提取软件"
                            btnType: MButton.FBtnType.Ordinary
                            Layout.columnSpan: 2
                            Layout.fillWidth: true
                            onClicked: {
                                AppDetailControl.extractApp(AppDetailControl.packageName, "");
                            }
                        }

                        MButton {
                            text: "冻结软件"
                            btnType: MButton.FBtnType.Ordinary
                            Layout.columnSpan: 2
                            Layout.fillWidth: true
                            onClicked: {
                                AppDetailControl.freezeApp(AppDetailControl.packageName);
                            }
                        }

                        MButton {
                            text: "解冻软件"
                            btnType: MButton.FBtnType.Ordinary
                            Layout.columnSpan: 2
                            Layout.fillWidth: true
                            onClicked: {
                                AppDetailControl.unfreezeApp(AppDetailControl.packageName);
                            }
                        }

                        MButton {
                            text: "卸载软件"
                            btnType: MButton.FBtnType.Warning
                            Layout.columnSpan: 2
                            Layout.fillWidth: true
                            onClicked: {
                                AppDetailControl.uninstallApp(AppDetailControl.packageName);
                            }
                        }

                        MButton {
                            text: "清除数据"
                            btnType: MButton.FBtnType.Warning
                            Layout.columnSpan: 2
                            Layout.fillWidth: true
                            onClicked: {
                                AppDetailControl.clearData(AppDetailControl.packageName);
                            }
                        }

                        MButton {
                            text: "强行停止"
                            btnType: MButton.FBtnType.Warning
                            Layout.columnSpan: 2
                            Layout.fillWidth: true
                            onClicked: {
                                AppDetailControl.stopApp(AppDetailControl.packageName);
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
                        MButton {
                            text: "启动"
                            btnType: MButton.FBtnType.Suggest
                            Layout.fillWidth: true
                            Layout.preferredWidth: 40
                            Layout.columnSpan: 2
                            onClicked: {
                                AppDetailControl.startApp(AppDetailControl.packageName);
                            }
                        }
                    }
                }
            }

            MWrapper {
                id: installWrapper
                property var flags: [false, false, false, false]
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
                                    status: installWrapper.flags[index] 

                                    onStatusChanged: {
                                        installWrapper.flags[index] = status
                                    }
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
                                installAppSelectAppDialog.open();
                            }
                        }
                    }

                    MButton {
                        Layout.fillWidth: true
                        text: "开始安装"
                        btnType: MButton.FBtnType.Suggest
                        onClicked: {
                            AppDetailControl.installApp(appPackagePathEdit.editItem.text, installWrapper.flags[0], installWrapper.flags[1], installWrapper.flags[2], installWrapper.flags[3]);
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
            name: "支持SDK:"
            info: "-"
        }

        Component.onCompleted: {
            softDetailInfoModel.get(0).info = AppDetailControl.versionCode;
            softDetailInfoModel.get(1).info = AppDetailControl.installDate;
            softDetailInfoModel.get(2).info = AppDetailControl.installUser;
            softDetailInfoModel.get(3).info = AppDetailControl.minSdk + "-" + AppDetailControl.targetSdk;
            AppDetailControl.valueChanged.connect(function () {
                softDetailInfoModel.get(0).info = AppDetailControl.versionCode;
                softDetailInfoModel.get(1).info = AppDetailControl.installDate;
                softDetailInfoModel.get(2).info = AppDetailControl.installUser;
                softDetailInfoModel.get(3).info = AppDetailControl.minSdk + "-" + AppDetailControl.targetSdk;
            });
        }
    }
    ListModel {
        id: softDetailBtnModel
        ListElement {
            name: "提取软件"
        }
        ListElement {
            name: "冻结软件"
        }
        ListElement {
            name: "解冻软件"
        }
        ListElement {
            name: "卸载软件"
        }
        ListElement {
            name: "清除数据"
        }
        ListElement {
            name: "强行停止"
        }
    }

    ListModel {
        id: installSoftOptionModel
        ListElement {
            name: "允许覆盖安装"
        }
        ListElement {
            name: "安装到sdcard"
        }
        ListElement {
            name: "允许降级安装"
        }
        ListElement {
            name: "授予所有运行时权限"
        }
    }
    FileDialog {
        id: installAppSelectAppDialog
        title: "Select a File"
        fileMode: FileDialog.OpenFile
        onAccepted: {
            appPackagePathEdit.editItem.text = installAppSelectAppDialog.currentFile;
        }
    }
}
