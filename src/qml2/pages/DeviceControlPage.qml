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
    title: "Device Control"
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
                    text: "Connect a device to access controls"
                    color: FluTheme.fontSecondaryColor
                }
            }

            FluFrame {
                Layout.fillWidth: true
                Layout.leftMargin: 20
                Layout.rightMargin: 20
                visible: !!device
                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 16
                    spacing: 8
                    FluText { text: "Media Control"; font: FluTextStyle.Subtitle }
                    GridLayout {
                        columns: 6
                        columnSpacing: 6
                        rowSpacing: 6
                        Layout.fillWidth: true
                        Repeater {
                            model: ["Prev","Stop","Play","Next","Vol-","Vol+"]
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
                    anchors.fill: parent
                    anchors.margins: 16
                    spacing: 8
                    FluText { text: "Key Simulation"; font: FluTextStyle.Subtitle }
                    GridLayout {
                        columns: 5
                        columnSpacing: 6
                        rowSpacing: 6
                        Layout.fillWidth: true
                        Repeater {
                            model: ["Menu","Home","Back","Power","Br+","Br-","Shutdown","Reboot","Rec","FB","Shot","Mute","Asst","Home^","End$"]
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
                headerText: "Broadcast Simulation"
                content: GridLayout {
                    columns: 3
                    columnSpacing: 6
                    rowSpacing: 6
                    Repeater {
                        model: ["Network","ScreenOn","ScreenOff","BatLow","BatOK","Boot","StoreLow","StoreOK","Install","WiFi1","WiFi2","BatChg","IME","PwrOn","PwrOff","DreamOn","DreamOff","Wallpap","Headset","Unmount","Mount","PwrSave"]
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
                headerText: "Battery Spoofing"
                content: ColumnLayout {
                    spacing: 8
                    RowLayout {
                        FluTextBox { id: bl; placeholderText: "Level"; text: "100"; Layout.preferredWidth: 100 }
                        FluButton { text: "Set"; onClicked: BatteryDisguise.setBatteryLevel(parseInt(bl.text) || 100) }
                    }
                    RowLayout {
                        FluButton { text: "Stop Charge"; Layout.fillWidth: true; onClicked: BatteryDisguise.stopCharge() }
                        FluButton { text: "Restore"; Layout.fillWidth: true; onClicked: BatteryDisguise.restoreCharge() }
                        FluButton { text: "USB No Chg"; Layout.fillWidth: true; onClicked: BatteryDisguise.connectButNoCharge() }
                        FluButton { text: "Reset All"; Layout.fillWidth: true; onClicked: BatteryDisguise.restoreAll() }
                    }
                }
            }

            FluExpander {
                Layout.fillWidth: true
                Layout.leftMargin: 20
                Layout.rightMargin: 20
                visible: !!device
                headerText: "Resolution & DPI"
                content: GridLayout {
                    columns: 3
                    columnSpacing: 8
                    rowSpacing: 8
                    FluTextBox { id: we; placeholderText: "Width"; text: ResolutionControl.screenWidth; Layout.fillWidth: true }
                    FluTextBox { id: he; placeholderText: "Height"; text: ResolutionControl.screenHeight; Layout.fillWidth: true }
                    FluTextBox { id: de; placeholderText: "DPI"; text: ResolutionControl.dpi; Layout.fillWidth: true }
                    FluButton { text: "Restore"; Layout.fillWidth: true; onClicked: ResolutionControl.restore() }
                    FluButton {
                        text: "Apply"
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
                headerText: "File Transfer"
                content: ColumnLayout {
                    spacing: 8
                    RowLayout {
                        FluTextBox { id: fs; placeholderText: "Source"; Layout.fillWidth: true }
                        FluButton { text: "Browse"; onClicked: fd.open() }
                    }
                    RowLayout {
                        FluTextBox { id: fdst; placeholderText: "Target"; text: "/sdcard/"; Layout.fillWidth: true }
                        FluButton { text: "Transfer"; onClicked: FileTransfer.transmission(fs.text, fdst.text) }
                    }
                }
            }

            FluExpander {
                Layout.fillWidth: true
                Layout.leftMargin: 20
                Layout.rightMargin: 20
                visible: !!device
                headerText: "Keyboard Sharing"
                content: Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 56
                    radius: 8
                    color: FluTheme.dark ? Qt.rgba(1,1,1,0.04) : Qt.rgba(0,0,0,0.03)
                    border.color: kb.focus ? FluTheme.primaryColor : Qt.rgba(0,0,0,0.1)
                    border.width: 1
                    FluText {
                        anchors.centerIn: parent
                        text: kb.focus ? "Listening..." : "Click to type on device"
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
                headerText: "Launch Activity"
                content: ColumnLayout {
                    spacing: 8
                    FluTextBox { id: an; placeholderText: "Activity"; text: "com.tencent.mm/.ui.LauncherUI"; Layout.fillWidth: true }
                    FluTextBox { id: aa; placeholderText: "Arguments"; Layout.fillWidth: true }
                    FluButton { text: "Launch"; Layout.fillWidth: true; onClicked: StartActivity.start(an.text, aa.text) }
                }
            }
        }
    }

    FileDialog { id: fd; title: "Select File"; fileMode: FileDialog.OpenFile; onAccepted: fs.text = String(currentFile) }
}
