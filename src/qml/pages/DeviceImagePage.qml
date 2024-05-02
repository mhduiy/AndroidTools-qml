import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import MFloat

Item {
    id: root
    Layout.minimumWidth: 780
    ColorConstants {
        id: colorConstants
    }
    RowLayout {
        anchors.fill: parent
        anchors.rightMargin: 10
        anchors.bottomMargin: 10
        Rectangle {
            Layout.fillHeight: true
            Layout.preferredWidth: 300
            Layout.maximumWidth: 300
            Layout.minimumWidth: 300
            radius: 10
            color: "black"
        }
        ColumnLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            MFrame {
                Layout.fillWidth: true
                Layout.preferredHeight: 120
                wrapperColor: Qt.rgba(255, 255, 255, 0.65)
                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 10
                    Text {
                        text: "实时数据"
                        font.pixelSize: 16
                        Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        MLabel {
                            vMargin: 4
                            hMargin: 4
                            text: "码率"
                        }
                        Item {
                            Layout.fillWidth: true
                        }

                        Text {
                            Layout.preferredWidth: 100
                            text: "123Mbps"
                        }
                    }
                    RowLayout {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 30
                        MLabel {
                            vMargin: 4
                            hMargin: 4
                            text: "帧率"
                        }
                        Item {
                            Layout.fillWidth: true
                        }
                        Text {
                            Layout.preferredWidth: 100
                            text: "60fps"
                        }
                    }
                    RowLayout {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 30
                        MLabel {
                            vMargin: 4
                            hMargin: 4
                            text: "坐标"
                        }
                        Item {
                            Layout.fillWidth: true
                        }
                        Text {
                            Layout.preferredWidth: 100
                            text: "1920, 1080"
                        }
                    }
                }
            }
            MFrame {
                Layout.fillWidth: true
                Layout.preferredHeight: 80
                wrapperColor: Qt.rgba(255, 255, 255, 0.65)
                GridLayout {
                    anchors.fill: parent
                    anchors.margins: 10
                    columns: 3
                    MButton {
                        Layout.fillWidth: true
                        text: "电源"
                    }
                    MButton {
                        Layout.fillWidth: true
                        text: "V +"
                    }
                    MButton {
                        Layout.fillWidth: true
                        text: "V -"
                    }
                    MButton {
                        Layout.fillWidth: true
                        text: "Menu"
                    }
                    MButton {
                        Layout.fillWidth: true
                        text: "Home"
                    }
                    MButton {
                        Layout.fillWidth: true
                        text: "Back"
                    }
                }
            }

            MFrame {
                Layout.fillWidth: true
                Layout.fillHeight: true
                wrapperColor: Qt.rgba(255, 255, 255, 0.65)
                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 10
                    RowLayout {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 30
                        Text {
                            text: "帧率限制"
                        }
                        Item {
                            Layout.fillWidth: true
                        }

                        MLineEdit {
                            Layout.preferredWidth: 140
                            placeholderText: "帧率(fps)"
                        }

                        MButton {
                            Layout.preferredWidth: 80
                            text: "设置"
                            btnType: MButton.FBtnType.Suggest
                        }
                    }
                    RowLayout {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 30
                        Text {
                            text: "码率限制"
                        }
                        Item {
                            Layout.fillWidth: true
                        }

                        MLineEdit {
                            Layout.preferredWidth: 140
                            placeholderText: "码率(Mbps)"
                        }

                        MButton {
                            Layout.preferredWidth: 80
                            text: "设置"
                            btnType: MButton.FBtnType.Suggest
                        }
                    }
                    RowLayout {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 30
                        Text {
                            text: "屏幕截图"
                        }
                        Item {
                            Layout.fillWidth: true
                        }
                        MLineEdit {
                            Layout.preferredWidth: 140
                            placeholderText: "保存路径"
                        }

                        MButton {
                            Layout.preferredWidth: 80
                            text: "点击截图"
                            btnType: MButton.FBtnType.Suggest
                        }
                    }
                    RowLayout {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 30
                        Text {
                            text: "屏幕录制"
                        }
                        Item {
                            Layout.fillWidth: true
                        }
                        MLineEdit {
                            Layout.preferredWidth: 140
                            placeholderText: "保存路径"
                        }

                        MButton {
                            text: "开始录制"
                            Layout.preferredWidth: 80
                            btnType: MButton.FBtnType.Suggest
                        }
                    }
                    RowLayout {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 30
                        Text {
                            text: "按键宏录制"
                        }
                        Item {
                            Layout.fillWidth: true
                        }

                        MLineEdit {
                            Layout.preferredWidth: 140
                            placeholderText: "宏名称"
                        }

                        MButton {
                            Layout.preferredWidth: 80
                            text: "开始录制"
                            btnType: MButton.FBtnType.Suggest
                        }
                    }
                    RowLayout {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 30
                        Text {
                            text: "执行宏"
                        }
                        Item {
                            Layout.fillWidth: true
                        }

                        MLineEdit {
                            Layout.preferredWidth: 140
                            placeholderText: "宏名称"
                        }

                        MButton {
                            Layout.preferredWidth: 80
                            text: "开始执行"
                            btnType: MButton.FBtnType.Suggest
                        }
                    }
                    RowLayout {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 30
                        Text {
                            text: "宏重复执行"
                        }
                        Item {
                            Layout.fillWidth: true
                        }

                        MSwitchButton {
                            Layout.preferredWidth: 50
                        }
                    }
                    RowLayout {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 30
                        Text {
                            text: "宏执行间隔"
                        }
                        Item {
                            Layout.fillWidth: true
                        }

                        MLineEdit {
                            placeholderText: "间隔(ms)"
                            Layout.preferredWidth: 140
                        }
                    }
                }
            }
        }
    }
}
