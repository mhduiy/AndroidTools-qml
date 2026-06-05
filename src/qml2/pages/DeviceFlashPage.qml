import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import FluentUI
import FastBootDeviceManager 1.0
import FlashTools 1.0

FluContentPage {
    id: page
    title: "刷机工具"
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
        FileDialog { id: fdo; title: "选择文件"; onAccepted: ip.text = String(currentFile) }
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
                    FluText { text: "无Fastboot设备"; font: FluTextStyle.Title; Layout.alignment: Qt.AlignHCenter }
                    FluText { text: "重启进入Fastboot模式后点击刷新"; color: FluTheme.fontSecondaryColor; Layout.alignment: Qt.AlignHCenter }
                    RowLayout {
                        Layout.alignment: Qt.AlignHCenter
                        spacing: 8
                        FluButton { text: "刷新"; onClicked: FastBootDeviceManager.updateDevices() }
                        FluButton { text: "重启到Fastboot"; onClicked: FastBootDeviceManager.rebootToFastBoot("") }
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
                    FluText { text: "设备: " + deviceCode; font: FluTextStyle.BodyStrong }
                    Item { Layout.fillWidth: true }
                    FluButton { text: "刷新"; onClicked: FastBootDeviceManager.updateDevices() }
                    FluButton { text: "重启FB"; onClicked: FastBootDeviceManager.rebootToFastBoot("") }
                    FluButton { text: "系统"; onClicked: FastBootDeviceManager.rebootToSystem("") }
                    FluButton { text: "关机"; onClicked: FastBootDeviceManager.powerOff("") }
                }
            }

            RowLayout {
                Layout.fillWidth: true
                Layout.leftMargin: 20
                Layout.rightMargin: 20
                spacing: 12
                visible: !!deviceCode

                FlashCard {
                    ti: "临时启动"
                    ds: "不刷入的情况下启动镜像"
                    Layout.fillWidth: true
                    ph: "boot.img"
                    bt: "启动"
                    onA: function(p) { FlashTools.startBoot("", p) }
                }
                FlashCard {
                    ti: "刷入"
                    ds: "将镜像刷入分区"
                    Layout.fillWidth: true
                    sp: true
                    ph: "image.img"
                    pp: "boot"
                    bt: "刷入"
                    onAP: function(p, pt) { FlashTools.flash("", pt, p) }
                }
                FlashCard {
                    ti: "擦除"
                    ds: "清除分区"
                    Layout.fillWidth: true
                    op: true
                    pp: "cache"
                    bt: "擦除"
                    onAPO: function(pt) { FlashTools.clear("", pt) }
                }
            }

            FluExpander {
                Layout.fillWidth: true
                Layout.leftMargin: 20
                Layout.rightMargin: 20
                visible: !!deviceCode
                headerText: "高级"
                content: ColumnLayout {
                    spacing: 8
                    RowLayout {
                        FluTextBox { id: sp; placeholderText: "脚本路径"; Layout.fillWidth: true }
                        FluButton { text: "..."; onClicked: sd.open() }
                        FluTextBox { id: sw; placeholderText: "工作目录"; Layout.fillWidth: true }
                        FluButton { text: "执行"; onClicked: FlashTools.executeScript("", sp.text, sw.text) }
                    }
                    RowLayout {
                        FluTextBox { id: xp; placeholderText: "小米线刷包"; Layout.fillWidth: true }
                        FluButton { text: "..."; onClicked: xd.open() }
                        FluButton { text: "刷入"; onClicked: FlashTools.flashToXiaomi("", xp.text) }
                    }
                    RowLayout {
                        FluTextBox { id: zi; placeholderText: "压缩包"; Layout.fillWidth: true }
                        FluButton { text: "..."; onClicked: zd.open() }
                        FluTextBox { id: zo; placeholderText: "输出"; Layout.fillWidth: true }
                        FluButton { text: "解压"; onClicked: FlashTools.unzip(zi.text, zo.text) }
                    }
                }
            }
        }
    }

    FileDialog { id: sd; title: "选择脚本"; onAccepted: sp.text = String(currentFile) }
    FileDialog { id: xd; title: "选择线刷包"; onAccepted: xp.text = String(currentFile) }
    FileDialog { id: zd; title: "选择压缩包"; onAccepted: zi.text = String(currentFile) }
}
