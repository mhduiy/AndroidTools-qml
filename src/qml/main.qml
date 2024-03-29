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


    Image {
        id: backgroundImage
        anchors.fill: parent
        source: "qrc:/res/backgroundImage.jpeg"
    }

    RowLayout {
        anchors.fill: parent
        Rectangle { // left
            id: leftSidebar
            Layout.fillHeight: true
            width: 200
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            color: "transparent"

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
                        Layout.alignment: Qt.AlignTop

                        RowLayout {
                            spacing: 4
                            Layout.margins: 5
                            Layout.alignment: Qt.AlignTop
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
                            Layout.alignment: Qt.AlignTop
                            anchors.margins: 4
                            Layout.preferredHeight: parent.height * 0.4
                        }

                        Item {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                        }

                        ControlListview {
                            id: controlListView
                            Layout.fillWidth: true
                            anchors.margins: 4
                            Layout.alignment: Qt.AlignBottom
                            Layout.preferredHeight: parent.height * 0.5

                            onCurrentIndexChanged: {
                                switch (currentIndex) {
                                case 0:
                                    rightContainter.replace(deviceInfoPage)
                                    break
                                case 1:
                                    rightContainter.replace(deviceControlPage)
                                    break
                                case 2:
                                    rightContainter.replace(deviceAppPage)
                                    break
                                case 3:
                                    rightContainter.replace(deviceFlashPage)
                                    break
                                case 4:
                                    rightContainter.replace(deviceImagePage)
                                    break
                                case 5:
                                    rightContainter.replace(deviceTerminalPage)
                                    break
                                }
                            }
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

        StackView {
            id: rightContainter
            Layout.alignment: Qt.AlignTop
            Layout.fillWidth: true
            Layout.fillHeight: true

            initialItem: deviceInfoPage

            replaceExit: Transition {
                PropertyAnimation { target: pushExit.view; property: "opacity"; to: 0; duration: 300 }
            }
            replaceEnter: Transition {
                PropertyAnimation { target: pushEnter.view; property: "opacity"; to: 1; duration: 500; easing.type: Easing.OutQuart}
                PropertyAnimation { target: pushEnter.view; property: "y"; from: 100; to: 0; duration: 500; easing.type: Easing.OutQuart}
            }
        }
        Component {
            id: deviceInfoPage
            Page {
                background: {}
                DeviceInfoPage {
                    anchors.fill: parent
                }
            }
        }

        Component {
            id: deviceControlPage
            Page {
                background: {}
                DeviceControlPage {
                    anchors.fill: parent
                }
            }

        }

        Component {
            id: deviceAppPage
            Page {
                background: {}
                DeviceInfoPage {
                    anchors.fill: parent
                }
            }

        }

        Component {
            id: deviceFlashPage
            Page {
                background: {}
                Rectangle {
                    anchors.fill: parent
                    color: "yellow"
                }
            }

        }

        Component {
            id: deviceImagePage
            Page {
                background: {}
                DeviceInfoPage {
                    anchors.fill: parent
                }
            }

        }

        Component {
            id: deviceTerminalPage

            Page {
                background: {}
                Rectangle {
                    anchors.fill: parent
                    color: "blue"
                }
            }

        }
    }
}
