import QtQuick
import MFloat
import QtQuick.Controls
import QtQuick.Layouts

import "../components/"
import NotificationControl 1.0
import WallPaperModel 1.0
import WallpaperHelper 1.0

Item {
    property bool isCurrentPage: false
    Layout.minimumWidth: 780
    ScrollView {
        anchors.fill: parent
        anchors.rightMargin: 10
        ScrollBar.vertical: ScrollBar {
            policy: ScrollBar.AlwaysOff
        }

        ColumnLayout {
            width: parent.parent.width
            spacing: 2

            MFrame {
                Layout.preferredWidth: parent.width
                Layout.fillWidth: true
                Layout.preferredHeight: wallpaperSettingItemTitle.height + wallpaperSettingItemContent.height
                wrapperColor: Qt.rgba(255, 255, 255, 0.65)
                Item {
                    id: wallpaperSettingItemTitle
                    width: parent.width
                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.leftMargin: 10
                    height: 60
                    ColumnLayout {
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        height: wallpaperSettingItemTitle_Titel.height + wallpaperSettingItemTitle_des.height
                        Label {
                            id: wallpaperSettingItemTitle_Titel
                            text: "壁纸设置"
                            font.pixelSize: 16
                        }
                        Label {
                            id: wallpaperSettingItemTitle_des
                            text: "设置AndroidTools的背景壁纸"
                            font.pixelSize: 12
                            color: "gray"
                        }
                    }
                }

                Item {
                    id: wallpaperSettingItemContent
                    width: parent.width
                    anchors.left: parent.left
                    anchors.top: wallpaperSettingItemTitle.bottom
                    height: 150

                    ListView {
                        id: wallpaperListView
                        anchors.fill: parent
                        anchors.margins: 10
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
                controlItemWidth: 200
                controlItem: Component {
                    Slider {
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
            }
            SettingItem {
                title: "壁纸模糊度"
                description: "设置壁纸的模糊半径"
                controlItemWidth: 200
                controlItem: Component {
                    Slider {
                        id: slider
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
            }

            SettingItem {
                title: "夜间模式"
                description: "将所有控件设置为深色主题"
                controlItemWidth: 50
                controlItem: Component {
                    MSwitchButton {

                    }
                }
            }

            Repeater {
                model: 16
                SettingItem {
                    title: "夜间模式"
                    description: "将所有控件设置为深色主题"
                    controlItemWidth: 30
                    controlItem: Component {
                        MLoadIndicator {
                            id: loadIndicator
                            Component.onCompleted: {
                                loadIndicator.start()
                            }
                        }
                    }
                }
            }
        }
    }
}

