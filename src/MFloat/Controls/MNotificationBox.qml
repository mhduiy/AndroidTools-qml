import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

import NotificationController
import App

Item {
    id: root

    property var blurTarget

    width: 350
    height: 50
    y: 40
    z: 1000

    transitions: [
        Transition {
            from: "close"; to: "open"
            PropertyAnimation {
                target: root
                properties: "x"
                duration: 500
                easing.type: Easing.OutExpo
            }
            PropertyAnimation {
                target: root
                properties: "opacity"
                duration: 500
            }
        },
        Transition {
            from: "open"; to: "close"
            PropertyAnimation {
                target: root
                properties: "x"
                duration: 300
                easing.type: Easing.InExpo
            }
            PropertyAnimation {
                target: root
                properties: "opacity"
                duration: 300
            }
        }
    ]

    states: [
        State {
            name: "open"
            PropertyChanges {
                target: root
                opacity: 1.0
                x: Window.width - root.width - 20
            }
        },
        State {
            name: "close"
            PropertyChanges {
                target: root
                opacity: 0.0
                x: Window.width
            }
        }
    ]

    state: "close"

    ColorConstants {
        id: colorConstants
    }

    Item {
        id: control
        anchors.fill: parent

        ShaderEffectSource {
            id: blurSource
            anchors.fill: parent
            sourceItem: root.blurTarget
            sourceRect: Qt.rect(root.x, root.y, root.width, root.height)
            visible: false
        }

        MultiEffect {
            anchors.fill: blurSource
            source: blurSource
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
            color: {
                if (NotificationController.type === NotificationController.Info) {
                    return Qt.rgba(0 / 255, 165 / 255, 255 / 255, 0.3)
                } else {
                    return Qt.rgba(214 / 255, 62 / 255, 62 / 255, 0.3)
                }
            }
            border.color: Qt.darker("white", 1.8)
            layer.enabled: true
        }

        Behavior on scale {
            PropertyAnimation {
                duration: 300
                easing.type: Easing.OutExpo
            }
        }
        Item {
            anchors.fill: parent

            Item {
                anchors.fill: parent
                Text {
                    id: titleControl
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    anchors.topMargin: 5
                    text: NotificationController.title
                    font.bold: true
                    font.pixelSize: 18
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    Behavior on opacity {
                        NumberAnimation {
                            duration: 200
                        }
                    }
                }

                Text {
                    id: contentControl
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    anchors.bottomMargin: 5
                    text: NotificationController.content
                    color: "white"
                    font.pixelSize: 12
                    opacity: 0.8
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    Behavior on opacity {
                        NumberAnimation {
                            duration: 200
                        }
                    }
                }

            }


            Canvas {
                id: closeIcon
                anchors.centerIn: parent
                width: 20
                height: 20
                opacity: 0
                property real margin: 2.0
                onPaint: {
                    var ctx = getContext("2d");
                    ctx.strokeStyle = "white"
                    ctx.lineWidth = 2;
                    ctx.beginPath();
                    ctx.moveTo(margin, margin);
                    ctx.lineTo(width - margin, width - margin);
                    ctx.moveTo(width - margin, margin);
                    ctx.lineTo(margin, width - margin);
                    ctx.stroke();
                }
                Behavior on opacity {
                    NumberAnimation {
                        duration: 200
                    }
                }
            }

            MouseArea {
                id: mouseArea
                anchors.fill: parent
                hoverEnabled: true
                onEntered: {
                    control.scale = 1.05
                    titleControl.opacity = 0
                    contentControl.opacity = 0
                    closeIcon.opacity = 1
                    closeTimer.stop()
                }

                onExited: {
                    control.scale = 1.0
                    titleControl.opacity = 1
                    contentControl.opacity = 1
                    closeIcon.opacity = 0
                    closeTimer.start()
                }

                onClicked: {
                    titleControl.opacity = 1
                    contentControl.opacity = 1
                    closeIcon.opacity = 0
                    root.state = "close"
                }
            }
        }
    }

    Timer {
        id: closeTimer
        interval: NotificationController.duration
        onTriggered: {
            root.state = "close"
        }
    }

    Connections {
        target: NotificationController
        function onRequestNotification() {
            root.state = "open"
            closeTimer.start()
        }
    }
}
