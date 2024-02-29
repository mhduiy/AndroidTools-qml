import QtQuick
import QtQuick.Controls
import MFloat

ApplicationWindow {
    width: 640
    height: 480
    visible: true
    title: qsTr("MFloat_Example")

    Column {
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 10
        Row {
            spacing: 10
            MButton {
                text: "普通按钮"
                btnType:MButton.FBtnType.Ordinary
                onClicked: {
                    bar.value = Math.floor(Math.random() * 100) + 1
                }
            }
            MButton {
                text: "建议按钮"
                btnType:MButton.FBtnType.Suggest
            }
            MButton {
                text: "警告按钮"
                btnType:MButton.FBtnType.Warning
            }
        }

        MSwitchButton {
            onStatusChanged: {
                if (status) {
                    console.log("Switch is ON");
                } else {
                    console.log("Switch is OFF");
                }
            }
        }

        MProgressBar {
            id: bar
            showText: true
            value: 30
        }

        Row {
            spacing: 10
            MLabel {
                text: "你好世界"
                hMargin: 20
                vMargin: 10
                radius: 10
            }
            MLabel {
                text: "MHDUIY"
                rectColor: "#000000"
                textColor: "#ffffff"
                hMargin: 15
                vMargin: 10
                radius: 10
            }
        }

        MCheckBox {
            // tristate: true
            checkState: Qt.Unchecked
        }
    }
}
