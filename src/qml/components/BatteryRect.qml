import QtQuick

Item {
    id: root
    property bool charging: false
    property int level: 80

    // 背景
    Rectangle {
        clip: true
        id: backRect
        anchors.fill: parent
        radius: 10
        color: Qt.lighter(indi.color, 1.8)

        property string warningColor: "#E74C3C"
        property string lowColor: "#E67E22"
        property string midColor: "#3498DB"
        property string goodColor: "#2ECC71"

        Rectangle {
            id: indi
            anchors.bottom: parent.bottom
            width: parent.width
            radius: parent.radius
            height: parent.height * root.level / 100.0
            color: {
                if (root.level < 10) {
                    return backRect.warningColor;
                } else if (root.level < 30) {
                    return backRect.lowColor;
                } else if (root.level < 50) {
                    return backRect.midColor;
                } else {
                    return backRect.goodColor;
                }
            }

            Behavior on height {
                PropertyAnimation {
                    duration: 300
                    easing.type: Easing.OutExpo
                }
            }

            Behavior on color {
                PropertyAnimation {
                    duration: 300
                }
            }

            Rectangle {
                x: 0
                y: root.height
                width: root.width
                height: 100

                // 使用一个线性渐变
                gradient: Gradient {
                    GradientStop {
                        position: 0.0
                        color: Qt.rgba(255, 255, 255, 0.4)
                    }
                    GradientStop {
                        position: 1.0
                        color: "transparent"
                    }
                }

                NumberAnimation on y {
                    from: root.height
                    to: 0
                    duration: 5000
                    loops: Animation.Infinite
                    easing.type: Easing.OutQuad
                }

                NumberAnimation on opacity {
                    from: 1
                    to: 0
                    duration: 5000
                    loops: Animation.Infinite
                }
            }
        }

        Text {
            anchors.centerIn: parent
            color: "white"
            font.family: "黑体"
            font.pixelSize: 16
            font.bold: true
            text: root.level + "%"
        }
    }
}
