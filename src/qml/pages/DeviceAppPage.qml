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
        anchors.bottomMargin: 10
        anchors.rightMargin: 10

        MFrame {
            Layout.minimumWidth: 300
            Layout.preferredWidth: parent.width * 0.4
            Layout.fillHeight: true
            wrapperColor: Qt.rgba(255, 255, 255, 0.65)

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 10

                RowLayout {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 40
                    Text {
                        text: "软件列表"
                        font.pixelSize: 18
                        font.family: "黑体"
                    }
                    Item {
                        Layout.fillWidth: true
                    }

                    ComboBox {
                        id: softListComboBox
                        Layout.minimumWidth: 80
                        model: [
                            "所有应用",
                            "系统应用",
                            "第三方应用"
                        ]
                    }
                }

                ListView {
                    id: softListView
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    model: 100
                    spacing: 2
                    clip: true
                    delegate: Component{
                        Rectangle {
                            width: ListView.view.width
                            height: 40
                            border.width: 1
                            color: "transparent"
                            border.color: "gray"
                            radius: 10
                        }
                    }
                }
            }
        }

        ColumnLayout {
            Layout.fillHeight: true
            Layout.preferredWidth: parent.width * 0.5
            MFrame {
                Layout.fillWidth: true
                Layout.preferredHeight: parent.height * 0.6
                wrapperColor: Qt.rgba(255, 255, 255, 0.65)

                GridLayout {
                    anchors.fill: parent
                    anchors.margins: 10
                    columns: 6
                    Text {
                        text: "包名:"
                        font.pixelSize: 16
                        Layout.preferredWidth: 80
                    }
                    Text {
                        text: "com.test.test.test.test"
                        font.pixelSize: 16
                        Layout.columnSpan: 5
                        Layout.fillWidth: true
                    }
                    Repeater {
                        model: softDetailInfoModel
                        MLabel {
                            text: model.name
                            Layout.row: index / 3 + 1
                            Layout.column: (index % 3) * 2
                            Layout.preferredWidth: 60
                            rectColor: colorConstants.suggestClickedColor
                            vMargin: 4
                            hMargin: 4
                        }
                    }

                    Repeater {
                        model: softDetailInfoModel
                        Text {
                            text: model.info
                            Layout.row: index / 3 + 1
                            Layout.column: (index % 3) * 2 + 1
                            Layout.fillWidth: true
                        }
                    }

                    Repeater {
                        model: softDetailBtnModel
                        MButton {
                            text: model.name
                            btnType: index < 3 ? MButton.FBtnType.Ordinary : MButton.FBtnType.Warning
                            Layout.columnSpan: 2
                            Layout.fillWidth: true
                        }
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
                    Text {
                        text: "安装软件"
                        font.pixelSize: 18
                        font.family: "黑体"
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        Repeater {
                            model: installSoftOptionModel
                            RowLayout {
                                Layout.fillWidth: true
                                Layout.preferredWidth: 30
                                spacing: 10
                                MCheckBox {

                                }
                                Text {
                                    text: model.name
                                    font.pixelSize: 12
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                }
                            }
                        }
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        MLineEdit {
                            Layout.fillWidth: true
                            placeholderText: "安装包路径"
                        }
                        MButton {
                            text: "选择路径"
                            btnType: MButton.FBtnType.Ordinary
                        }
                    }

                    MButton {
                        Layout.fillWidth: true
                        text: "开始安装"
                        btnType: MButton.FBtnType.Suggest
                    }
                }
            }
        }
    }

    ListModel {
        id: softDetailInfoModel

        ListElement {
            name: "版本号:"
            info: "-"
        }
        ListElement {
            name: "安装时间:"
            info: "-"
        }
        ListElement {
            name: "安装者:"
            info: "-"
        }
        ListElement {
            name: "目标SDK:"
            info: "-"
        }
        ListElement {
            name: "最小SDK:"
            info: "-"
        }
        ListElement {
            name: "APPID"
            info: "-"
        }
    }
    ListModel {
        id: softDetailBtnModel
        ListElement { name: "提取软件" }
        ListElement { name: "冻结软件" }
        ListElement { name: "解冻软件" }
        ListElement { name: "卸载软件" }
        ListElement { name: "清除数据" }
        ListElement { name: "强行停止" }
    }

    ListModel {
        id: installSoftOptionModel
        ListElement { name: "允许覆盖安装" }
        ListElement { name: "安装到sdcard" }
        ListElement { name: "允许降级安装" }
        ListElement { name: "授予所有运行时权限" }
    }
}
