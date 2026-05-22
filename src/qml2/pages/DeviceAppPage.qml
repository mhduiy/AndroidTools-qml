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
    title: "App Manager"

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
                FluText { text: "No device connected"; font: FluTextStyle.Title; Layout.alignment: Qt.AlignHCenter }
                FluText { text: "Connect a device to browse and manage apps"; color: FluTheme.fontSecondaryColor; Layout.alignment: Qt.AlignHCenter }
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
                        model: ["Third Party", "System", "All"]
                        currentIndex: 0
                        onCurrentIndexChanged: AppDetailControl.softListType = currentIndex
                    }
                    FluTextBox {
                        id: searchBox
                        Layout.fillWidth: true
                        placeholderText: "Search..."
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

                FluButton { Layout.fillWidth: true; text: "+ Install APK"; onClicked: installDialog.open() }
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
                    text: AppDetailControl.packageName || "Select an app from the list"
                    font: FluTextStyle.Subtitle
                }

                GridLayout {
                    columns: 2; columnSpacing: 16; rowSpacing: 6
                    visible: !!AppDetailControl.packageName
                    InfoRow { label: "Version"; value: AppDetailControl.versionCode > 0 ? String(AppDetailControl.versionCode) : "--" }
                    InfoRow { label: "Installed"; value: AppDetailControl.installDate || "--" }
                    InfoRow { label: "Min SDK"; value: AppDetailControl.minSdk || "--" }
                    InfoRow { label: "Target SDK"; value: AppDetailControl.targetSdk || "--" }
                }

                RowLayout { spacing: 8; visible: !!AppDetailControl.packageName
                    FluButton { text: "Extract"; Layout.fillWidth: true; onClicked: AppDetailControl.extractApp() }
                    FluButton { text: "Freeze"; Layout.fillWidth: true; onClicked: AppDetailControl.freezeApp() }
                    FluButton { text: "Unfreeze"; Layout.fillWidth: true; onClicked: AppDetailControl.unfreezeApp() }
                }
                RowLayout { spacing: 8; visible: !!AppDetailControl.packageName
                    FluButton { text: "Uninstall"; Layout.fillWidth: true; onClicked: AppDetailControl.uninstallApp() }
                    FluButton { text: "Clear Data"; Layout.fillWidth: true; onClicked: AppDetailControl.clearData() }
                    FluButton { text: "Force Stop"; Layout.fillWidth: true; onClicked: AppDetailControl.stopApp() }
                }
            }
        }
    }

    component InfoRow: RowLayout {
        property string label; property string value
        FluText { text: label + ":"; font: FluTextStyle.Caption; color: FluTheme.fontSecondaryColor; Layout.preferredWidth: 80 }
        FluText { text: value; font: FluTextStyle.Body }
    }

    FileDialog { id: installDialog; title: "Select APK"; nameFilters: ["APK files (*.apk)"]; onAccepted: AppDetailControl.installApp(String(currentFile)) }
}
