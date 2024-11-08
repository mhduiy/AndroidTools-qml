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

    ColorConstants {
        id: colorConstants
    }

    MNotificationBox {
        id: notificationBox
        blurTarget: rootRect
    }

    Rectangle {
        id: rootRect
        color: "transparent"
        anchors.fill: parent

        Component.onCompleted: {
            rootRect.opacity = 1
            rootRect.scale = 1
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
                    root.close()
                }
                onRequestMinWindow: {
                    root.showMinimized()
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
        anchors.fill: parent
        backImage: rootRect
    }
}
