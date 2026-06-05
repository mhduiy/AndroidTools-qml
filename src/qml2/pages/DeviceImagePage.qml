import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import FluentUI
import ConnectManager 1.0
import MirrorScene 1.0
import Resource 1.0
import ScrcpyConfig 1.0
import ImageFrameItem 1.0
import ImageDetailTools 1.0
import NotificationController 1.0

FluContentPage {
    id: page
    title: "屏幕镜像"

    property var device: ConnectManager.cutADBDevice

    RowLayout {
        anchors.fill: parent
        anchors.margins: 12
        spacing: 12

        // ---- 无设备 ----
        FluFrame {
            Layout.fillWidth: true
            Layout.fillHeight: true
            visible: !device
            FluText {
                anchors.centerIn: parent
                text: "连接设备以开始屏幕镜像"
                color: FluTheme.fontSecondaryColor
            }
        }

        // ---- 视频区域 ----
        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true
            visible: !!device
            color: "#000"
            radius: 8

            MirrorScene {
                id: video
                anchors { fill: parent; margins: 4 }
                ImageFrameItem {
                    imageFrame: video.image
                    hasAlphaChannel: false
                    anchors.fill: parent
                }

                readonly property int winHome: 0
                readonly property int winPortrait: 1
                readonly property int winLandscape: 2

                onCppGenerateEvents: function(request, data) {
                    switch (request) {
                    case "MIRROR_START":
                        Resource.mirror = 1
                        Resource.scene = video.winPortrait
                        break
                    case "MIRROR_FINISHED":
                        Resource.mirror = 0
                        break
                    case "DISPLAY_ORIENTATION_CHANGED":
                        Resource.scene = Resource.orientation === 0 ? video.winPortrait : video.winLandscape
                        break
                    }
                }
            }
        }

        // ---- 控制面板 ----
        FluFrame {
            Layout.preferredWidth: 220
            Layout.fillHeight: true
            visible: !!device

            ColumnLayout {
                anchors { fill: parent; margins: 14 }
                spacing: 10

                FluText { text: "控制"; font: FluTextStyle.BodyStrong }

                RowLayout {
                    FluButton {
                        text: Resource.mirror ? "停止" : "开始"
                        Layout.fillWidth: true
                        onClicked: {
                            if (Resource.mirror) Resource.qmlRequest("REQUEST_MIRROR_FINISH", "")
                            else Resource.qmlRequest("REQUEST_MIRROR_START", "")
                        }
                    }
                    FluButton {
                        text: "截图"
                        Layout.fillWidth: true
                        onClicked: ImageDetailTools.shotScreen("")
                    }
                }

                FluDivider {}

                FluText { text: "帧率"; font: FluTextStyle.Caption; color: FluTheme.fontSecondaryColor }
                RowLayout {
                    FluTextBox { id: fpsInput; text: ScrcpyConfig.maxFps; Layout.fillWidth: true }
                    FluButton { text: "设置"; onClicked: { ScrcpyConfig.maxFps = parseInt(fpsInput.text)||30; restartMirror() } }
                }

                FluText { text: "码率 (Kbps)"; font: FluTextStyle.Caption; color: FluTheme.fontSecondaryColor }
                RowLayout {
                    FluTextBox { id: rateInput; text: ScrcpyConfig.kBitRate; Layout.fillWidth: true }
                    FluButton { text: "设置"; onClicked: { ScrcpyConfig.kBitRate = parseInt(rateInput.text)||8000; restartMirror() } }
                }

                FluDivider {}

                FluText { text: "录制"; font: FluTextStyle.Caption; color: FluTheme.fontSecondaryColor }
                RowLayout {
                    FluTextBox { id: recPath; text: ScrcpyConfig.recordOutPath; Layout.fillWidth: true }
                    FluButton {
                        text: "录制"; Layout.fillWidth: true
                        onClicked: {
                            ScrcpyConfig.recordOutPath = recPath.text
                            if(!Resource.mirror) Resource.qmlRequest("REQUEST_MIRROR_START","")
                            NotificationController.send("Recording","Disconnect to save",0,3000)
                        }
                    }
                }
            }
        }
    }

    function restartMirror() {
        if (Resource.mirror) {
            Resource.qmlRequest("REQUEST_MIRROR_FINISH", "")
            Resource.qmlRequest("REQUEST_MIRROR_START", "")
        }
    }
}
