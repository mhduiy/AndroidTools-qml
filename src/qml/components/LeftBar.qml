import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import MFloat
import ADBControl
import QtQuick.Effects
import Qt5Compat.GraphicalEffects

Item {
    id: leftSidebar
    width: 220
    height: Window.height
    x: hoverHandler.hovered ? 0 : -width + 20
    y: 0
    property var backImage
    Behavior on x {
        NumberAnimation {
            duration: 800
            easing.type: Easing.OutQuint
        }
    }
    Rectangle {
        id: backgroundRectangle
        anchors.margins: 10
        anchors.fill: parent
        radius: 10
        color: "transparent"

        ShaderEffectSource {
            id: effectSource
            anchors.fill: parent
            sourceItem: backImage
            sourceRect: Qt.rect(leftSidebar.x, leftSidebar.y, leftSidebar.width, leftSidebar.height)
            visible: false
        }

        MultiEffect {
            anchors.fill: effectSource
            source: effectSource
            autoPaddingEnabled: false
            blurEnabled: true
            blurMax: 64
        }

        Rectangle {
            anchors.fill: parent
            radius: 10
            color: Qt.rgba(255, 255, 255, 0.3)
            border.color: Qt.darker("white", 1.8)
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

    Dialog {
        id: wirelessConnectDialog
        title: "开始无线连接"
        width: 400
        height: 150
        modal: Qt.ApplicationModal
        visible: false
        anchors.centerIn: Overlay.overlay

        header: Item {}

        background: Item {
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
                blurMax: 64
            }

            Rectangle {
                anchors.fill: parent
                radius: 20
                color: Qt.rgba(255, 255, 255, 0.8)
                border.color: Qt.darker("white", 1.8)
            }
        }

        ColumnLayout {
            anchors.fill: parent
            Text {
                text: "1. 打开开发者模式中的无线调试"
                font.family: "黑体"
                font.pixelSize: 16
            }
            Text {
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
            Text {
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


