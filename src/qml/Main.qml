import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import MFloat
import Qt5Compat.GraphicalEffects

import "./components/"
import "./pages/"
import ADBControl 1.0
import WallpaperHelper 1.0

ApplicationWindow {
    id: root
    width: 1000
    height: 610
    visible: true
    title: "AndroidTools"
    flags: Qt.Window | Qt.FramelessWindowHint | Qt.WindowMinimizeButtonHint
    color: "transparent"

    function handleWindowStateChanged(newState) {
        if (newState === Qt.WindowNoState) {
            rootRect.scale = 1
            rootRect.opacity = 1
            rootRect.prepareDo = Main.PrepareDo.ShowWindow
        }
    }

    onWindowStateChanged: handleWindowStateChanged(windowState)

    ColorConstants {
        id: colorConstants
    }

    MNotificationBox {
        id: notificationBox
    }

    enum PrepareDo {
        ShowWindow = 0,
        CloseWindow = 1,
        MinWindow = 2
    }

    Rectangle {
        id: rootRect
        color: "transparent"
        anchors.fill: parent
        scale: 0.8
        opacity: 0.2
        property int prepareDo: Main.PrepareDo.ShowWindow

        Behavior on opacity {
            PropertyAnimation {
                duration: 300
            }
        }
        Behavior on scale {
            PropertyAnimation {
                id: applicationCloseOpenAni
                duration: 500
                easing.type: Easing.OutBack
            }
        }
        Component.onCompleted: {
            rootRect.opacity = 1
            rootRect.scale = 1
        }

        Connections {
            target: applicationCloseOpenAni
            function onRunningChanged(running) {
                if (!running) {
                    if (rootRect.prepareDo === Main.PrepareDo.CloseWindow) {
                        root.close()
                    } else if (rootRect.prepareDo === Main.PrepareDo.MinWindow) {
                        root.showMinimized()
                        rootRect.opacity = 0.8
                    } else {
                        root.showNormal()
                    }
                }
            }
        }

        Rectangle {
            id: imageRect
            anchors.fill: parent
            color: "transparent"

            Image {
                id: backgroundImage
                fillMode: Image.PreserveAspectCrop
                source: WallpaperHelper.wallpaper
                visible: false
            }
            Rectangle {
                id: mask
                anchors.fill: parent
                radius: 10
                visible: false
            }
            OpacityMask {
                anchors.fill: parent
                source: backgroundImage
                maskSource: mask
            }
        }

        ColumnLayout {
            anchors.fill: parent
            TitleBar {
                Layout.fillWidth: true
                Layout.preferredHeight: 30
                Layout.minimumHeight: 30
                Layout.maximumHeight: 30

                onRequestCloseWindow: {
                    rootRect.scale = 0.8
                    rootRect.opacity = 0
                    rootRect.prepareDo = Main.PrepareDo.CloseWindow
                }
                onRequestMinWindow: {
                    rootRect.scale = 0.8
                    rootRect.opacity = 0
                    rootRect.prepareDo = Main.PrepareDo.MinWindow
                }

                onRequestMoveWindow: {
                    root.x = tarX
                    root.y = tarY
                }
            }

            RowLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Rectangle { // left
                    id: leftSidebar
                    Layout.fillHeight: true
                    width: 200
                    Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                    color: "transparent"

                    RowLayout {
                        width: parent.width
                        height: parent.height

                        Item {
                            height: parent.height
                            width: 5

                        }

                        Item {
                            Layout.fillWidth: true
                            Layout.fillHeight: true

                            ColumnLayout {
                                anchors.fill: parent
                                Layout.alignment: Qt.AlignTop

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
                                        scale: 0.4
                                        opacity: 0

                                        Behavior on scale {
                                            PropertyAnimation {
                                                duration: 2000
                                                easing.type: Easing.OutBack
                                            }
                                        }
                                        Behavior on opacity {
                                            PropertyAnimation {
                                                duration: 2000
                                            }
                                        }

                                        Component.onCompleted: {
                                            scale = 1.0
                                            opacity = 1.0
                                        }
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
                                    anchors.margins: 4
                                    Layout.alignment: Qt.AlignBottom
                                    Layout.preferredHeight: parent.height * 0.65

                                    onCurrentIndexChanged: {
                                        switch (currentIndex) {
                                        case 0:
                                            rightContainter.push(deviceInfoPage)
                                            break
                                        case 1:
                                            rightContainter.push(deviceControlPage)
                                            break
                                        case 2:
                                            rightContainter.push(deviceAppPage)
                                            break
                                        case 3:
                                            rightContainter.push(deviceFlashPage)
                                            break
                                        case 4:
                                            rightContainter.push(deviceImagePage)
                                            break
                                        case 5:
                                            rightContainter.push(deviceTerminalPage)
                                            break
                                        case 6:
                                            rightContainter.push(settingPage)
                                            break
                                        }
                                    }
                                }
                            }
                        }

                        Rectangle {
                            Layout.alignment: Qt.AlignTop
                            width: 2
                            height: parent.height
                            color: "gray"
                        }
                    }
                }

                StackView {
                    id: rightContainter
                    Layout.alignment: Qt.AlignTop
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    initialItem: deviceInfoPage

                    pushExit: Transition {
                        PropertyAnimation { target: pushExit.view; property: "opacity"; to: 0; duration: 300 }
                    }
                    pushEnter: Transition {
                        PropertyAnimation { target: pushEnter.view; property: "opacity"; to: 1; duration: 600;}
                        PropertyAnimation { target: pushEnter.view; property: "y"; from: 200; to: 0; duration: 600; easing.type: Easing.OutQuart}
                    }
                }
                Component {
                    id: deviceInfoPage
                    Page {
                        background: {}
                        DeviceInfoPage {
                            anchors.fill: parent
                        }
                    }
                }

                Component {
                    id: deviceControlPage
                    Page {
                        background: {}
                        DeviceControlPage {
                            anchors.fill: parent
                        }
                    }

                }

                Component {
                    id: deviceAppPage
                    Page {
                        background: {}
                        DeviceAppPage {
                            anchors.fill: parent
                        }
                    }

                }

                Component {
                    id: deviceFlashPage
                    Page {
                        background: {}
                        DeviceFlashPage {
                            anchors.fill: parent
                        }
                    }

                }

                Component {
                    id: deviceImagePage
                    Page {
                        background: {}
                        DeviceImagePage {
                            anchors.fill: parent
                        }
                    }

                }

                Component {
                    id: deviceTerminalPage

                    Page {
                        background: {}
                        DeviceTerminalPage {
                            anchors.fill: parent
                        }
                    }
                }

                Component {
                    id: settingPage
                    Page {
                        background: {}
                        SettingPage {
                            anchors.fill: parent
                        }
                    }

                }
            }

        }


    }

    Dialog {
        id: wirelessConnectDialog
        title: "开始无线连接"
        width: 500
        height: 300
        modal: Qt.ApplicationModal
        visible: false
        anchors.centerIn: parent
        ColumnLayout {
            anchors.fill: parent
            Text {
                text: "1. 打开开发者模式中的无线调试"
                font.family: "黑体"
                font.pixelSize: 16
            }
            Text {
                text: "2. 点击使用配对码配对设备，并填写IP和配对码，若已经配对则忽略"
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
