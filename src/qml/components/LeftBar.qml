import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import MFloat
import ADBControl
import QtQuick.Effects
import Qt5Compat.GraphicalEffects
import App

Item {
    id: sideBar
    property var backImage
    property int radius: 12

    MultiEffect {
        id: effect
        anchors.fill: parent
        source: backImage
        maskEnabled: true
        maskSource: markItem
        autoPaddingEnabled: false
        blurEnabled: true
        blur: 0.8
        blurMax: 64
    }

    Item {
        id: markItem
        anchors.fill: parent
        layer.enabled: true
        layer.smooth: true
        visible: false

        Rectangle {
            x: sidebarControl.x + sidebarControlContent.x
            y: sidebarControl.y + sidebarControlContent.y
            width: sidebarControlContent.width
            height: sidebarControlContent.height
            radius: sideBar.radius
        }
    }


    Item {
        id: sidebarControl
        property bool startAniStatus: false
        width: 220
        height: Window.height
        x: hoverHandler.hovered  ? 0 : startAniStatus ?  -width + 80 : -width + 20
        y: 0

        Behavior on x {
            NumberAnimation {
                duration: 800
                easing.type: Easing.OutQuint
            }
        }
        Rectangle {
            id: sidebarControlContent
            anchors.margins: 10
            anchors.fill: parent
            radius: sideBar.radius
            color: App.midColor
            border.color: App.borderColor

            Behavior on color {
                ColorAnimation {
                    duration: 200
                }
            }

            ColumnLayout {
                anchors.fill: parent
                Layout.alignment: Qt.AlignTop
                anchors.margins: 10

                RowLayout {
                    spacing: 4
                    Layout.margins: 5
                    Layout.alignment: Qt.AlignTop
                    Text {
                        id: greeterText
                        font.bold: true
                        font.pixelSize: 18
                        color: "#E67E22"
                        text: "Hi! 下午好"
                    }

                    Item {
                        Layout.fillWidth: true
                    }

                    MButton {
                        text: "无线连接"
                        btnType: MButton.FBtnType.Ordinary
                        onClicked: {
                            wirelessConnectDialog.open()
                        }
                    }
                }

                DeviceListview {
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignTop
                    anchors.margins: 4
                    Layout.preferredHeight: parent.height * 0.30
                }

                Item {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }

                ControlListview {
                    id: controlListView
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignBottom

                    onCurrentIndexChanged: {
                        mainStackLayout.currentIndex = currentIndex
                    }
                }
            }
        }

        HoverHandler {

            id: hoverHandler
        }

        Component.onCompleted: {
            startAniTimer.start()
        }

        Timer {
            id: startAniTimer
            interval: 1500
            repeat : true
            onTriggered: {
                if (!sidebarControl.startAniStatus) {
                    sidebarControl.startAniStatus = true
                    repeat = false
                } else {
                    sidebarControl.startAniStatus = false
                }
            }
        }
    }

    Dialog {
        id: wirelessConnectDialog
        title: "开始无线连接"
        width: 450
        height: 200
        modal: Qt.ApplicationModal
        visible: false
        anchors.centerIn: Overlay.overlay
        header: Item {}

        background: Item {
            id: backItem
            state: wirelessConnectDialog.visible ? "visible" : "hidden"

            Behavior on scale {
                PropertyAnimation {
                    duration: 600
                    easing.type: Easing.OutExpo
                }
            }

            states: [
                State {
                    name: "visible"
                    PropertyChanges { target: backItem; scale: 1.0;}
                    PropertyChanges { target: dialogContent; scale: 1.0; opacity: 1.0}
                },
                State {
                    name: "hidden"
                    PropertyChanges { target: backItem; scale: 0;}
                    PropertyChanges { target: dialogContent; scale: 0; opacity: 0}
                }
            ]

            ShaderEffectSource {
                id: dialogSource
                anchors.fill: parent
                sourceItem: backImage
                sourceRect: Qt.rect(wirelessConnectDialog.x, wirelessConnectDialog.y, wirelessConnectDialog.width, wirelessConnectDialog.height)
                visible: false
            }

            MultiEffect {
                anchors.fill: dialogSource
                source: dialogSource
                autoPaddingEnabled: false
                blurEnabled: true
                maskEnabled: true
                maskSource: markRect
                blur: 0.8
                blurMax: 64
            }

            Rectangle {
                id: markRect
                anchors.fill: parent
                radius: 12
                color: App.midColor
                border.color: App.borderColor
                layer.enabled: true
                Behavior on color {
                    ColorAnimation {
                        duration: 200
                    }
                }
            }
        }

        ColumnLayout {
            id: dialogContent
            anchors.fill: parent
            anchors.margins: 10

            Behavior on scale {
                PropertyAnimation {
                    duration: 800
                    easing.type: Easing.OutExpo
                }
            }

            Behavior on opacity {
                PropertyAnimation {
                    duration: 1000
                }
            }

            Label {
                text: "1. 打开开发者模式中的无线调试"
                font.family: "黑体"
                font.pixelSize: 16
            }
            Label {
                text: "2. 点击使用配对码配对设备，并填写IP和配对码"
                wrapMode: Text.WrapAnywhere
                font.family: "黑体"
                font.pixelSize: 16
                width: 100
            }
            RowLayout {
                MLineEdit {
                    id: pairEdit
                    placeholderText: "IP:PORT"
                    Layout.minimumWidth: 200
                }
                MLineEdit {
                    id: pairCodeEdit
                    placeholderText: "配对码"
                    Layout.minimumWidth: 100
                }
                MButton {
                    btnType: MButton.FBtnType.Suggest
                    text: "配对设备"
                    onClicked: {
                        ADBControl.pairDevice(pairEdit.editItem.text, pairCodeEdit.editItem.text)
                    }
                }
            }
            Label {
                text: "3. 返回上一级，查看IP和端口，并填写"
                font.family: "黑体"
                font.pixelSize: 16
            }
            RowLayout {
                MLineEdit {
                    id: connectEdit
                    placeholderText: "IP:PORT"
                    Layout.minimumWidth: 200
                }
                MButton {
                    btnType: MButton.FBtnType.Suggest
                    text: "连接设备"
                    onClicked: {
                        ADBControl.connectDevice(connectEdit.editItem.text)
                    }
                }
            }
        }
    }
}


