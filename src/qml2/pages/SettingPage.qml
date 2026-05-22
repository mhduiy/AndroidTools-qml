import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import FluentUI
import WallPaperModel 1.0
import WallpaperHelper 1.0
import OtherSettingsHandler 1.0
import App 1.0
import ConnectManager 1.0

FluContentPage {
    id: page
    title: "Settings"

    Timer {
        id: restartTimer
        interval: 1500
        onTriggered: ConnectManager.startCheckDevice()
    }

    ScrollView {
        anchors.fill: parent
        contentWidth: availableWidth

        ColumnLayout {
            width: parent.width
            spacing: 12

            // ---- Wallpaper ----
            FluFrame {
                Layout.fillWidth: true
                Layout.leftMargin: 20
                Layout.rightMargin: 20
                Layout.topMargin: 12

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 16
                    spacing: 10

                    FluText { text: "Wallpaper"; font: FluTextStyle.Subtitle }

                    ScrollView {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 90
                        contentHeight: availableHeight
                        RowLayout {
                            spacing: 8
                            Repeater {
                                model: WallPaperModel
                                delegate: Rectangle {
                                    width: 110; height: 75; radius: 8; color: "transparent"
                                    border { color: index === WallPaperModel.currentIndex ? FluTheme.primaryColor : Qt.rgba(0,0,0,0.1); width: 2 }
                                    Image {
                                        anchors { fill: parent; margins: 4 }
                                        source: model.url; fillMode: Image.PreserveAspectCrop; asynchronous: true
                                    }
                                    MouseArea { anchors.fill: parent; onClicked: WallPaperModel.setCurrentIndex(index) }
                                }
                            }
                            FluButton { text: "+ Add"; Layout.preferredWidth: 80; Layout.preferredHeight: 75; onClicked: WallpaperHelper.requestAddCustomWallpaper() }
                        }
                    }

                    RowLayout {
                        FluText { text: "Opacity"; Layout.preferredWidth: 140 }
                        FluSlider {
                            id: opacitySlide; Layout.fillWidth: true
                            from: 0.1; to: 1.0; value: WallpaperHelper.opacity
                            onMoved: WallpaperHelper.opacity = value
                        }
                        FluText { text: Math.round(opacitySlide.value*100)+"%"; Layout.preferredWidth: 40 }
                    }

                    RowLayout {
                        FluText { text: "Blur"; Layout.preferredWidth: 140 }
                        FluSlider {
                            id: blurSlide; Layout.fillWidth: true
                            from: 0; to: 64; value: WallpaperHelper.blurRadius
                            onMoved: WallpaperHelper.blurRadius = value
                        }
                        FluText { text: Math.round(blurSlide.value); Layout.preferredWidth: 40 }
                    }
                }
            }

            // ---- Appearance ----
            FluFrame {
                Layout.fillWidth: true
                Layout.leftMargin: 20
                Layout.rightMargin: 20

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 16
                    spacing: 12

                    FluText { text: "Appearance"; font: FluTextStyle.Subtitle }

                    RowLayout {
                        FluText { text: "Dark Mode"; Layout.preferredWidth: 140 }
                        FluToggleSwitch { checked: App.themeType === App.Dark; onClicked: App.setThemeType(checked ? App.Dark : App.Light) }
                    }

                    RowLayout {
                        FluText { text: "Panel Opacity"; Layout.preferredWidth: 140 }
                        FluSlider {
                            id: panelSlide; Layout.fillWidth: true
                            from: 0.1; to: 1.0; value: OtherSettingsHandler.wrapperOpacity
                            onMoved: OtherSettingsHandler.wrapperOpacity = value
                        }
                        FluText { text: Math.round(panelSlide.value*100)+"%"; Layout.preferredWidth: 40 }
                    }
                }
            }

            // ---- Advanced ----
            FluFrame {
                Layout.fillWidth: true
                Layout.leftMargin: 20
                Layout.rightMargin: 20

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 16
                    spacing: 12

                    FluText { text: "Advanced"; font: FluTextStyle.Subtitle }

                    RowLayout {
                        FluText { text: "OpenGL"; Layout.preferredWidth: 140 }
                        FluToggleSwitch { checked: OtherSettingsHandler.useOpenGL; onClicked: OtherSettingsHandler.useOpenGL = !OtherSettingsHandler.useOpenGL }
                        FluText { text: "(restart)"; font: FluTextStyle.Caption; color: FluTheme.fontSecondaryColor }
                    }

                    RowLayout {
                        FluText { text: "Refresh"; Layout.preferredWidth: 140 }
                        FluSlider {
                            id: refSlide; Layout.fillWidth: true
                            from: 1000; to: 30000; stepSize: 1000
                            value: OtherSettingsHandler.deviceRefreshInterval
                            onMoved: OtherSettingsHandler.deviceRefreshInterval = value
                        }
                        FluText { text: Math.round(refSlide.value/1000)+"s"; Layout.preferredWidth: 36 }
                    }

                    RowLayout {
                        FluText { text: "ADB Server"; Layout.preferredWidth: 140 }
                        FluButton {
                            text: "Restart"
                            onClicked: {
                                ConnectManager.killADBServer()
                                ConnectManager.startADBServer()
                                restartTimer.start()
                            }
                        }
                    }
                }
            }

            // ---- About ----
            FluFrame {
                Layout.fillWidth: true
                Layout.leftMargin: 20
                Layout.rightMargin: 20

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 16
                    spacing: 4

                    FluText { text: "About"; font: FluTextStyle.Subtitle }
                    FluText { text: "AndroidTools v0.1.0"; font: FluTextStyle.Body }
                    FluText { text: "Qt 6.x  ·  MIT License"; font: FluTextStyle.Caption; color: FluTheme.fontSecondaryColor }
                }
            }
        }
    }
}
