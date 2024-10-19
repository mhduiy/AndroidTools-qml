import QtQuick

Item {
    id: root
    anchors.horizontalCenter: parent.horizontalCenter
    y: 60
    z: 100

    enum Type {
        Info = 0,
        Warning = 1,
        Error = 2
    }

    property var type: MNotificationBox.Type.Info
    function send(text, type, duration) {
        if (duration <= 0) {
            return
        }

        content.text = text
        root.type = type
        root.state = "open"
        timer.interval = duration
        timer.start()
    }

    Timer {
        id: timer
        interval: 0
        repeat: false
        onTriggered: {
            root.state = "close"
        }
    }

    transitions: [
        Transition {
            from: "close"; to: "open"
            PropertyAnimation {
                target: root
                properties: "y, opacity"
                duration: 500
                easing.type: Easing.OutBack
            }
            PropertyAnimation {
                target: root
                properties: "width, height"
                duration: 500
                easing.type: Easing.InOutCirc
            }
            PropertyAnimation {
                target: contentRect
                properties: "opacity"
                duration: 500
                easing.type: Easing.InOutExpo
            }
        },
        Transition {
            from: "open"; to: "close"
            PropertyAnimation {
                target: root
                properties: "width, height"
                duration: 300
                easing.type: Easing.OutCirc
            }
            PropertyAnimation {
                target: contentRect
                properties: "opacity"
                duration: 300
                easing.type: Easing.OutCirc
            }
            PropertyAnimation {
                target: root
                properties: "y, opacity"
                duration: 300
                easing.type: Easing.InCirc
            }
        }
    ]

    states: [
        State {
            name: "open"
            PropertyChanges {
                target: root
                y: 60
                width: 350
                height: 60
                opacity: 1.0
            }
            PropertyChanges {
                target: contentRect
                opacity: 1.0
            }
        },
        State {
            name: "close"
            PropertyChanges {
                target: root
                y: 5
                width: 30
                height: 30
                opacity: 0.0
            }
            PropertyChanges {
                target: contentRect
                opacity: 0.0
            }
        }
    ]

    state: "close"

    Rectangle {
        id: backRect
        anchors.fill: parent
        color: "black"
        radius: 10
        clip: true
        Rectangle {
            anchors.fill: parent
            color: "transparent"
            id: contentRect
            Text {
                id: content
                anchors.fill: parent
                text: "Hello World"
                color: "white"
                font.pixelSize: 16
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            Image {
                width: 30
                height: 30
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                asynchronous: true
                sourceSize: Qt.size(2*width,2*height)
                source: {
                    if (type === MNotificationBox.Type.Info) {
                        return "../images/successIcon.png"
                    } else if (type === MNotificationBox.Type.Warning) {
                        return "../images/warningIcon.png"
                    } else {
                        return "../images/errorIcon.png"
                    }
                }
            }

            Rectangle {
                height: 30
                width: 30
                anchors.right: parent.right
                anchors.rightMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                radius: width / 2
                Image {
                    anchors.fill: parent
                    source: "../images/closeIcon.png"
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        root.state = "close"
                        timer.stop()
                    }
                    onEntered: {
                        parent.color = Qt.darker(parent.color, 1.2)
                    }
                    onExited: {
                        parent.color = "white"
                    }
                }
            }
        }
    }
}
