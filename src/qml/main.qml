import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import MFloat

import "./components/"

ApplicationWindow {
    id: root
    width: 1000
    height: 600
    visible: true
    title: "AndroidTools"

    Rectangle {
        id: leftSidebar
        width: 200
        height: parent.height

        RowLayout {
            anchors.fill: parent
            Item {
                Layout.fillHeight: true
                width: 5
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true

                ColumnLayout {
                    anchors.fill: parent
                    Layout.alignment: Qt.AlignCenter
                    RowLayout {
                        spacing: 4
                        Layout.margins: 5
                        Text {
                            id: greeterText
                            font.bold: true
                            font.pixelSize: 20
                            text: "下午好"
                        }

                        Item {
                            Layout.fillWidth: true
                        }

                        MButton {
                            text: "无线连接"
                            btnType: MButton.FBtnType.Ordinary
                        }
                    }

                    DeviceListview {
                        Layout.fillWidth: true
                        anchors.margins: 4
                        height: 200
                    }

                    Item {
                        Layout.fillWidth: true
                        height: 20
                    }

                    ControlListview {
                        Layout.fillWidth: true
                        anchors.margins: 4
                        height: 300
                    }
                }
            }

            Item {
                Layout.fillHeight: true
                width: 5
            }

            Rectangle {
                width: 1
                Layout.fillHeight: true
                color: "gray"
            }
        }
    }
}
