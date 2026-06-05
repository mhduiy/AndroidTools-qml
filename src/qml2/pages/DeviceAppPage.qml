import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import FluentUI
import ConnectManager 1.0
import SoftListModel 1.0
import AppDetailControl 1.0

FluContentPage {
    id: page
    title: "应用管理"

    property var device: ConnectManager.cutADBDevice

    RowLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 12

        // ---- 无设备 ----
        FluFrame {
            Layout.fillWidth: true
            Layout.fillHeight: true
            visible: !device
            ColumnLayout {
                anchors.centerIn: parent; spacing: 12
                FluText { text: "未连接设备"; font: FluTextStyle.Title; Layout.alignment: Qt.AlignHCenter }
                FluText { text: "连接设备以浏览和管理应用"; color: FluTheme.fontSecondaryColor; Layout.alignment: Qt.AlignHCenter }
            }
        }

        // ---- 应用列表 ----
        FluFrame {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredWidth: 320
            visible: !!device

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 8

                RowLayout {
                    FluComboBox {
                        id: filterCombo
                        model: ["第三方", "系统应用", "全部"]
                        currentIndex: 0
                        onCurrentIndexChanged: AppDetailControl.softListType = currentIndex
                    }
                    FluTextBox {
                        id: searchBox
                        Layout.fillWidth: true
                        placeholderText: "搜索..."
                    }
                }

                ListView {
                    id: appListView
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true
                    model: SoftListModel

                    delegate: Rectangle {
                        width: ListView.view.width; height: 46
                        color: appListView.currentIndex === index ? FluTheme.itemHoverColor : "transparent"
                        radius: 6

                        RowLayout {
                            anchors { fill: parent; margins: 8 }
                            spacing: 10
                            Rectangle {
                                width: 30; height: 30; radius: 6
                                color: FluTheme.dark ? Qt.rgba(1,1,1,0.08) : Qt.rgba(0,0,0,0.06)
                                FluText { anchors.centerIn: parent; text: model.appName ? model.appName.charAt(0).toUpperCase() : "?"; font: FluTextStyle.BodyStrong }
                            }
                            FluText { text: model.appName || model.packageName || "?"; Layout.fillWidth: true; elide: Text.ElideRight }
                            FluText { text: model.versionName || ""; font: FluTextStyle.Caption; color: FluTheme.fontSecondaryColor }
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: { appListView.currentIndex = index; AppDetailControl.updateInfo(model.packageName) }
                        }
                    }
                }

                FluButton { Layout.fillWidth: true; text: "+ 安装APK"; onClicked: installDialog.open() }
            }
        }

        // ---- 应用详情 ----
        FluFrame {
            Layout.fillWidth: true
            Layout.fillHeight: true
            visible: !!device

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 16
                spacing: 12

                FluText {
                    text: AppDetailControl.packageName || "从列表中选择应用"
                    font: FluTextStyle.Subtitle
                }

                GridLayout {
                    columns: 2; columnSpacing: 16; rowSpacing: 6
                    visible: !!AppDetailControl.packageName
                    InfoRow { label: "版本"; value: AppDetailControl.versionCode > 0 ? String(AppDetailControl.versionCode) : "--" }
                    InfoRow { label: "安装日期"; value: AppDetailControl.installDate || "--" }
                    InfoRow { label: "最低SDK"; value: AppDetailControl.minSdk || "--" }
                    InfoRow { label: "目标SDK"; value: AppDetailControl.targetSdk || "--" }
                }

                RowLayout { spacing: 8; visible: !!AppDetailControl.packageName
                    FluButton { text: "提取"; Layout.fillWidth: true; onClicked: AppDetailControl.extractApp() }
                    FluButton { text: "冻结"; Layout.fillWidth: true; onClicked: AppDetailControl.freezeApp() }
                    FluButton { text: "解冻"; Layout.fillWidth: true; onClicked: AppDetailControl.unfreezeApp() }
                }
                RowLayout { spacing: 8; visible: !!AppDetailControl.packageName
                    FluButton { text: "卸载"; Layout.fillWidth: true; onClicked: AppDetailControl.uninstallApp() }
                    FluButton { text: "清除数据"; Layout.fillWidth: true; onClicked: AppDetailControl.clearData() }
                    FluButton { text: "强制停止"; Layout.fillWidth: true; onClicked: AppDetailControl.stopApp() }
                }
            }
        }
    }

    component InfoRow: RowLayout {
        property string label; property string value
        FluText { text: label + ":"; font: FluTextStyle.Caption; color: FluTheme.fontSecondaryColor; Layout.preferredWidth: 80 }
        FluText { text: value; font: FluTextStyle.Body }
    }

    FileDialog { id: installDialog; title: "选择APK"; nameFilters: ["APK files (*.apk)"]; onAccepted: AppDetailControl.installApp(String(currentFile)) }
}
