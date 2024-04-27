import QtQuick
import MFloat
import QtQuick.Controls
import QtQuick.Layouts

import "../components/"
import NotificationControl 1.0

Item {
    Layout.minimumWidth: 780
    ScrollView {
        anchors.fill: parent
        anchors.topMargin: 10
        anchors.rightMargin: 10
        ScrollBar.vertical: ScrollBar {
            policy: ScrollBar.AlwaysOff
        }

        ColumnLayout {
            width: parent.parent.width
            spacing: 2

            SettingItem {
                id: wallpaperItem
                title: "壁纸"
                description: "设置AndroidTools的背景壁纸"
                controlItem: Component {
                    MButton {
                        text: "展开"
                        width: 50
                        btnType: MButton.FBtnType.Normal
                        onClicked: {
                            NotificationControl.send("展不开啊展不开", 1, 3000);
                        }
                    }
                }

                expandItem: Component {
                    Item {
                        anchors.fill: parent
                        ListView {
                            anchors.fill: parent
                            model: 10
                            delegate: ItemDelegate {
                                Item {
                                    width: 50
                                    height: 50
                                    Rectangle {
                                        anchors.fill: parent
                                        color: "#008c8c"
                                    }
                                }

                            }
                        }
                    }
                }
            }
            Repeater {
                model: 16
                SettingItem {
                    title: "夜间模式"
                    description: "将所有控件设置为深色主题"
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

