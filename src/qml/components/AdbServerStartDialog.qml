import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import MFloat
import QtQuick.Effects
import Qt5Compat.GraphicalEffects
import App
import NotificationController 1.0
import ConnectManager

Dialog {
    id: root
    title: "adb 服务启动中"
    width: 250
    height: 100
    modal: Qt.WindowModal
    visible: false
    anchors.centerIn: Overlay.overlay
    header: Item {}

    required property var backImage

    background: Item {
        id: backItem

        ShaderEffectSource {
            id: dialogSource
            anchors.fill: parent
            sourceItem: root.backImage
            sourceRect: Qt.rect(root.x, root.y, root.width, root.height)
            visible: false
        }

        MultiEffect {
            anchors.fill: dialogSource
            source: dialogSource
            autoPaddingEnabled: false
            blurEnabled: true
            maskEnabled: true
            maskSource: markRect
            blur: 0.8
            blurMax: 64
        }

        Rectangle {
            id: markRect
            anchors.fill: parent
            radius: 12
            color: App.midColor
            border.color: App.borderColor
            layer.enabled: true
            Behavior on color {
                ColorAnimation {
                    duration: 200
                }
            }
        }
    }

    ColumnLayout {
        id: dialogContent
        spacing: 20
        anchors.fill: parent
        anchors.margins: 10

        Label {
            text: "adb 服务启动中"
            font.pixelSize: 14
            color: App.windowTextColor
            Layout.alignment: Qt.AlignHCenter
        }

        MLoadIndicator {
            Layout.alignment: Qt.AlignHCenter
            Component.onCompleted: {
                start();
            }
        }
    }
}
