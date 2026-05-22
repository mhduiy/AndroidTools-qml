import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import FluentUI
import FastBootDeviceManager 1.0
import FlashTools 1.0

FluContentPage {
    id: page
    title: "Flash Tools"
    property string deviceCode: FastBootDeviceManager.currentDeviceCode

    // ---- FlashCard inline component ----
    component FlashCard: FluFrame {
        property string ti
        property string ds
        property bool sp: false
        property bool op: false
        property string ph
        property string pp
        property string bt
        property var onA: function(p) {}
        property var onAP: function(p, pt) {}
        property var onAPO: function(pt) {}

        Layout.preferredHeight: op ? 140 : 170

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 14
            spacing: 8
            FluText { text: ti; font: FluTextStyle.BodyStrong }
            FluText { text: ds; font: FluTextStyle.Caption; color: FluTheme.fontSecondaryColor }
            Item { Layout.fillHeight: true }
            RowLayout {
                spacing: 4
                visible: !op
                FluTextBox { id: ip; placeholderText: ph; Layout.fillWidth: true }
                FluButton { text: "..."; Layout.preferredWidth: 34; onClicked: fdo.open() }
            }
            RowLayout {
                spacing: 4
                visible: sp || op
                FluTextBox { id: pi; placeholderText: pp; Layout.fillWidth: true }
            }
            FluButton {
                Layout.fillWidth: true
                text: bt
                onClicked: {
                    if (op) onAPO(pi.text)
                    else if (sp) onAP(ip.text, pi.text)
                    else onA(ip.text)
                }
            }
        }
        FileDialog { id: fdo; title: "Select File"; onAccepted: ip.text = String(currentFile) }
    }

    ScrollView {
        anchors.fill: parent
        contentWidth: availableWidth

        ColumnLayout {
            width: parent.width
            spacing: 12

            FluFrame {
                Layout.fillWidth: true
                Layout.preferredHeight: 200
                Layout.leftMargin: 20
                Layout.rightMargin: 20
                Layout.topMargin: 12
                visible: !deviceCode
                ColumnLayout {
                    anchors.centerIn: parent
                    spacing: 10
                    FluText { text: "No Fastboot Device"; font: FluTextStyle.Title; Layout.alignment: Qt.AlignHCenter }
                    FluText { text: "Reboot into Fastboot mode, then click Refresh."; color: FluTheme.fontSecondaryColor; Layout.alignment: Qt.AlignHCenter }
                    RowLayout {
                        Layout.alignment: Qt.AlignHCenter
                        spacing: 8
                        FluButton { text: "Refresh"; onClicked: FastBootDeviceManager.updateDevices() }
                        FluButton { text: "Reboot to Fastboot"; onClicked: FastBootDeviceManager.rebootToFastBoot("") }
                    }
                }
            }

            FluFrame {
                Layout.fillWidth: true
                Layout.leftMargin: 20
                Layout.rightMargin: 20
                visible: !!deviceCode
                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 12
                    FluText { text: "Device: " + deviceCode; font: FluTextStyle.BodyStrong }
                    Item { Layout.fillWidth: true }
                    FluButton { text: "Refresh"; onClicked: FastBootDeviceManager.updateDevices() }
                    FluButton { text: "Reboot FB"; onClicked: FastBootDeviceManager.rebootToFastBoot("") }
                    FluButton { text: "System"; onClicked: FastBootDeviceManager.rebootToSystem("") }
                    FluButton { text: "PowerOff"; onClicked: FastBootDeviceManager.powerOff("") }
                }
            }

            RowLayout {
                Layout.fillWidth: true
                Layout.leftMargin: 20
                Layout.rightMargin: 20
                spacing: 12
                visible: !!deviceCode

                FlashCard {
                    ti: "Temp Boot"
                    ds: "Boot image without flashing"
                    Layout.fillWidth: true
                    ph: "boot.img"
                    bt: "Boot"
                    onA: function(p) { FlashTools.startBoot("", p) }
                }
                FlashCard {
                    ti: "Flash"
                    ds: "Flash image to partition"
                    Layout.fillWidth: true
                    sp: true
                    ph: "image.img"
                    pp: "boot"
                    bt: "Flash"
                    onAP: function(p, pt) { FlashTools.flash("", pt, p) }
                }
                FlashCard {
                    ti: "Erase"
                    ds: "Clear a partition"
                    Layout.fillWidth: true
                    op: true
                    pp: "cache"
                    bt: "Erase"
                    onAPO: function(pt) { FlashTools.clear("", pt) }
                }
            }

            FluExpander {
                Layout.fillWidth: true
                Layout.leftMargin: 20
                Layout.rightMargin: 20
                visible: !!deviceCode
                headerText: "Advanced"
                content: ColumnLayout {
                    spacing: 8
                    RowLayout {
                        FluTextBox { id: sp; placeholderText: "Script path"; Layout.fillWidth: true }
                        FluButton { text: "..."; onClicked: sd.open() }
                        FluTextBox { id: sw; placeholderText: "Work dir"; Layout.fillWidth: true }
                        FluButton { text: "Execute"; onClicked: FlashTools.executeScript("", sp.text, sw.text) }
                    }
                    RowLayout {
                        FluTextBox { id: xp; placeholderText: "Xiaomi package"; Layout.fillWidth: true }
                        FluButton { text: "..."; onClicked: xd.open() }
                        FluButton { text: "Flash"; onClicked: FlashTools.flashToXiaomi("", xp.text) }
                    }
                    RowLayout {
                        FluTextBox { id: zi; placeholderText: "Archive"; Layout.fillWidth: true }
                        FluButton { text: "..."; onClicked: zd.open() }
                        FluTextBox { id: zo; placeholderText: "Output"; Layout.fillWidth: true }
                        FluButton { text: "Extract"; onClicked: FlashTools.unzip(zi.text, zo.text) }
                    }
                }
            }
        }
    }

    FileDialog { id: sd; title: "Select Script"; onAccepted: sp.text = String(currentFile) }
    FileDialog { id: xd; title: "Select Package"; onAccepted: xp.text = String(currentFile) }
    FileDialog { id: zd; title: "Select Archive"; onAccepted: zi.text = String(currentFile) }
}
