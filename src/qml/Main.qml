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
    width: 900
    height: 600
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

            // 模糊效果
            GaussianBlur {
                id: blurEffect
                anchors.fill: parent
                source: backgroundImage
                radius: WallpaperHelper.blurRadius
                samples: radius
                visible: false
            }

            Image {
                id: backgroundImage
                anchors.fill: parent
                fillMode: Image.PreserveAspectCrop
                source: WallpaperHelper.wallpaper
                clip: true
                asynchronous: true
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
                source: blurEffect
                maskSource: mask
            }

            Rectangle{
                anchors.fill: parent
                radius: 10
                opacity: WallpaperHelper.opacity
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

                StackLayout {
                    id: mainStackLayout
                    Layout.alignment: Qt.AlignTop
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.leftMargin: 15
                    Layout.rightMargin: 5
                    Layout.bottomMargin: 5
                    property int preIndex: 0

                    onCurrentIndexChanged: {

                        for (var i = 0; i < mainStackLayout.count; i++) {
                            mainStackLayout.itemAt(i).isCurrentPage = (i === currentIndex);
                            if (i === currentIndex) {
                                mainStackLayout.itemAt(i).state = "pageShow";
                            } else {
                                mainStackLayout.itemAt(i).state = "prepareShow";
                            }


                        }
                    }

                    DeviceInfoPage {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Component.onCompleted: {
                            state: "pageShow"
                        }
                    }

                    DeviceControlPage {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                    }

                    DeviceAppPage {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                    }

                    DeviceFlashPage {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                    }

                    DeviceImagePage {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                    }

                    SettingPage {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                    }
                }
            }
        }

    }

    LeftBar {
        backImage: imageRect
    }
}
