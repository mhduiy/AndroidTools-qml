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

    ColumnLayout {
        anchors.fill: parent
        anchors.rightMargin: 10
        anchors.bottomMargin: 10

        MFrame {
            Layout.fillHeight: true
            Layout.fillWidth: true
            wrapperColor: Qt.rgba(0, 0, 0, 0.80)
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: 30
            MCheckBox {

            }
            Text {
                text: "切换到adb shell，进入设备shell环境"
            }
        }
        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: 50
            MButton {
                btnType: MButton.FBtnType.Ordinary
                text: "从剪切板粘贴"
                Layout.fillWidth: true
            }
            MButton {
                btnType: MButton.FBtnType.Ordinary
                text: "清除所有"
                Layout.fillWidth: true
            }
            MButton {
                btnType: MButton.FBtnType.Ordinary
                text: "打开系统终端"
                Layout.fillWidth: true
            }
        }
    }
}
