import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import MFloat
import TerminalPageTools 1.0
import QTermWidgetForQml 1.0

ItemPage {
    id: root
    ColorConstants {
        id: colorConstants
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.rightMargin: 10
        anchors.bottomMargin: 10

        MFrame {
            Layout.fillHeight: true
            Layout.fillWidth: true
            wrapperColor: Qt.rgba(255, 255, 255, 0.65)
            QTermWidgetForQml {
                anchors.margins: 10
                anchors.fill: parent
            }

        }

        MFrame {
            Layout.fillWidth: true
            Layout.preferredHeight: 80
            wrapperColor: Qt.rgba(255, 255, 255, 0.65)
            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 10             
                RowLayout {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Text {
                        text: "切换到adb shell，进入设备shell环境"
                    }
                    Item {
                        Layout.fillWidth: true
                    }

                    MSwitchButton {
                        onStatusChanged: {
                            if (status) {

                            } else {

                            }
                        }
                    }
                }
                RowLayout {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    MButton {
                        btnType: MButton.FBtnType.Ordinary
                        text: "从剪切板粘贴"
                        Layout.fillWidth: true
                        onClicked: {
                        }
                    }
                    MButton {
                        btnType: MButton.FBtnType.Ordinary
                        text: "清除所有"
                        Layout.fillWidth: true
                        onClicked: {

                        }
                    }
                    MButton {
                        btnType: MButton.FBtnType.Ordinary
                        text: "打开系统终端"
                        Layout.fillWidth: true
                        onClicked: {
                            TerminalPageTools.startSystemTerminal()
                        }
                    }
                }
            }
        }
    }
}
