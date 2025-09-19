import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import MFloat
import QtQuick.Effects
import Qt5Compat.GraphicalEffects
import App
import NotificationController 1.0
import ConnectManager

        Item {
    id: sideBar
    property var backImage
    property int radius: 12

    Rectangle {
        id: sidebarControlContent
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

            RowLayout {
                spacing: 4  
                Layout.margins: 5
                MButton {
                    text: "有线连接"
                    btnType: MButton.FBtnType.Ordinary
                    Layout.fillWidth: true
                    onClicked: {
                        wireConnectDialog.open()
                    }
                }

                MButton {
                    text: "无线连接"
                    btnType: MButton.FBtnType.Ordinary
                    Layout.fillWidth: true
                    onClicked: {
                        wirelessConnectDialog.open()
                    }
                }
            }

            DeviceListview {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignTop
                Layout.preferredHeight: parent.height * 0.27
                Layout.rightMargin: 10
                Layout.leftMargin: 10
            }

            ControlListview {
                id: controlListView
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignBottom
                Layout.rightMargin: 10
                Layout.leftMargin: 10

                onCurrentIndexChanged: {
                    mainStackLayout.currentIndex = currentIndex
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
                        ConnectManager.requestPairDevice(pairEdit.editItem.text, pairCodeEdit.editItem.text)
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
                        ConnectManager.requestConnectDevice(connectEdit.editItem.text)
                    }
                }
            }
        }
    }

    Dialog {
        id: wireConnectDialog
        title: "准备有线连接"
        width: 450
        height: 200
        modal: Qt.ApplicationModal
        visible: false
        anchors.centerIn: Overlay.overlay
        header: Item {}

        background: Item {
            id: wbackItem
            state: wireConnectDialog.visible ? "visible" : "hidden"

            Behavior on scale {
                PropertyAnimation {
                    duration: 600
                    easing.type: Easing.OutExpo
                }
            }

            states: [
                State {
                    name: "visible"
                    PropertyChanges { target: wbackItem; scale: 1.0;}
                    PropertyChanges { target: wireDialogContent; scale: 1.0; opacity: 1.0}
                },
                State {
                    name: "hidden"
                    PropertyChanges { target: wbackItem; scale: 0;}
                    PropertyChanges { target: wireDialogContent; scale: 0; opacity: 0}
                }
            ]

            ShaderEffectSource {
                id: wdialogSource
                anchors.fill: parent
                sourceItem: backImage
                sourceRect: Qt.rect(wireConnectDialog.x, wireConnectDialog.y, wireConnectDialog.width, wireConnectDialog.height)
                visible: false
            }

            MultiEffect {
                anchors.fill: wdialogSource
                source: wdialogSource
                autoPaddingEnabled: false
                blurEnabled: true
                maskEnabled: true
                maskSource: wmarkRect
                blur: 0.8
                blurMax: 64
            }

            Rectangle {
                id: wmarkRect
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
            id: wireDialogContent
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
                text: "1. 打开开发者模式中的有线调试"
                font.family: "黑体"
                font.pixelSize: 16
            }
            Label {
                text: "2. 几秒钟后同意电脑的连接"
                wrapMode: Text.WrapAnywhere
                font.family: "黑体"
                font.pixelSize: 16
            }
            Label {
                text: "3. adb 会自动连接到手机"
                wrapMode: Text.WrapAnywhere
                font.family: "黑体"
                font.pixelSize: 16
            }
        }
    }
}


