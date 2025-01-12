import QtQuick
import MFloat
import QtQuick.Controls
import QtQuick.Layouts

import "../components/"
import NotificationController 1.0
import WallPaperModel 1.0
import WallpaperHelper 1.0
import OtherSettingsHandler 1.0
import ADBControl 1.0
import App

ItemPage {
    ScrollView {
        anchors.fill: parent
        ScrollBar.vertical: ScrollBar {
            policy: ScrollBar.AlwaysOff
        }

        ColumnLayout {
            width: parent.parent.width
            spacing: 2

            MWrapper {
                Layout.preferredWidth: parent.width
                Layout.fillWidth: true
                Layout.preferredHeight: wallpaperSettingItemTitle.height + wallpaperSettingItemContent.height
                title: "壁纸设置"
                description: "设置AndroidTools的背景壁纸"
                titleRightContent: MButton {
                    text: "添加"
                    Layout.preferredWidth: 60
                    Layout.maximumWidth: 60
                    Layout.minimumWidth: 60
                    width: 60
                    btnType: MButton.FBtnType.Suggest
                    onClicked: {
                        WallpaperHelper.requestAddCustomWallpaper()
                    }
                }

                Item {
                    id: wallpaperSettingItemContent
                    width: parent.width
                    anchors.left: parent.left
                    anchors.top: wallpaperSettingItemTitle.bottom
                    implicitHeight: 150

                    ListView {
                        id: wallpaperListView
                        anchors.fill: parent
                        model: WallPaperModel
                        spacing: 4
                        orientation: ListView.Horizontal
                        clip: true
                        focus: true
                        currentIndex: WallPaperModel.currentIndex
                        delegate: Component {
                            Item {
                                id: wrapper
                                height: ListView.view.height
                                width: height * 1.4
                                Rectangle {
                                    anchors.fill: parent
                                    radius: 10
                                    color: "transparent"
                                    border.color: "gray"
                                    border.width: 1
                                    clip: true

                                    Image {
                                        id: wallpaperSourceImage
                                        anchors.fill: parent
                                        sourceSize: Qt.size(2 * width, 2 * height)
                                        source: model.url
                                        asynchronous: true
                                        fillMode: Image.PreserveAspectCrop
                                        clip: true

                                        onStatusChanged: {
                                            if (status === Image.Loading) {
                                                wallPaperLoadIndicator.start()
                                            } else {
                                                wallPaperLoadIndicator.stop();
                                            }
                                        }
                                    }

                                    MLabel {
                                        anchors.top: parent.top
                                        anchors.right: parent.right
                                        anchors.margins: 5
                                        text: model.title
                                        wrapperText.font.pixelSize: 10
                                        vMargin: 5
                                        hMargin: 5
                                    }

                                    Image {
                                        id: wallpaperCutImage
                                        anchors.centerIn: parent
                                        visible: wrapper.ListView.isCurrentItem
                                        width: 30
                                        height: 30
                                        source: "qrc:/res/successIcon.png"
                                        sourceSize: Qt.size(2 * width, 2 * height)
                                        Behavior on visible {
                                            PropertyAnimation {
                                                target: wallpaperCutImage
                                                from: wallpaperCutImage.visible ? 1 : 0
                                                to: wallpaperCutImage.visible ? 0 : 1
                                                property: "scale"
                                                duration: 300
                                                easing.type: Easing.OutBack
                                            }
                                        }
                                    }

                                    MLoadIndicator {
                                        id: wallPaperLoadIndicator
                                        anchors.centerIn: parent
                                        width: 30
                                        height: 30
                                    }

                                    MouseArea {
                                        anchors.fill: parent
                                        onClicked: {
                                            WallPaperModel.setCurrentIndex(index)
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            SettingItem {
                title: "壁纸不透明度"
                description: "设置壁纸白色遮罩的透明度"
                controlItem: Slider {
                    id: slider
                    width: 200
                    from: 0
                    value: WallpaperHelper.opacity
                    to: 1
                    snapMode: Slider.NoSnap
                    stepSize: 0.05

                    onValueChanged: {
                        WallpaperHelper.opacity = value
                    }
                }
            }
            SettingItem {
                title: "壁纸模糊度"
                description: "设置壁纸的模糊半径"
                controlItem: Slider {
                    width: 200
                    from: 0
                    value: WallpaperHelper.blurRadius
                    to: 50
                    snapMode: Slider.NoSnap
                    stepSize: 1

                    onValueChanged: {
                        WallpaperHelper.blurRadius = value
                    }
                }
            }

            SettingItem {
                title: "模块背景透明度"
                description: "设置每一个功能模块区域的透明度"
                controlItem: Slider {
                    width: 200
                    from: 0
                    value: OtherSettingsHandler.wrapperOpacity
                    to: 1
                    snapMode: Slider.NoSnap
                    stepSize: 0.01

                    onValueChanged: {
                        OtherSettingsHandler.wrapperOpacity = value
                    }
                }
            }

            SettingItem {
                title: "OpenGL渲染模式"
                description: "强制使用OpenGL渲染，如果设备镜像功能不可用，可尝试开启此开关，重启应用生效"
                controlItem: MSwitchButton {
                    status: OtherSettingsHandler.useOpenGL
                    onStatusChanged: {
                        OtherSettingsHandler.useOpenGL = status
                    }
                }
            }

            SettingItem {
                title: "重启adb服务"
                description: "若设备连接后程序无任何响应，可尝试重启adb服务解决"
                controlItem: MButton {
                    text: "重启"
                    onClicked: {
                        ADBControl.restartADB()
                    }
                }
            }

            SettingItem {
                title: "夜间模式"
                description: "将所有控件设置为深色主题"
                controlItem: MSwitchButton {
                    status: App.themeType
                    onStatusChanged: {
                        App.themeType = status
                    }
                }
            }
        }
    }
}

