import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import MFloat

import "./components/"
import "./pages/"

ApplicationWindow {
    id: root
    width: 1000
    height: 600
    visible: true
    title: "AndroidTools"

    RowLayout {
        anchors.fill: parent
        Rectangle { // left
            id: leftSidebar
            Layout.fillHeight: true
            width: 200
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop

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

                Rectangle {
                    Layout.alignment: Qt.AlignTop
                    width: 2
                    height: parent.height
                    color: "gray"
                }
            }
        }

        DeviceInfoPage {
            Layout.alignment: Qt.AlignTop
            id: rightContainer
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}
