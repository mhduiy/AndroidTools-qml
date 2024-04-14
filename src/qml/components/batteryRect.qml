import QtQuick

Item {
    width: 280
    height: 100
    property bool charging: false
    property int level: 80
    id: root

    // 背景
    Rectangle {
        id: backRect
        anchors.fill: parent
        radius: 10
        color: Qt.lighter(indi.color, 1.8)

        property string warningColor: "#E74C3C"
        property string lowColor: "#F1C40F"
        property string midColor: "#3498DB"
        property string goodColor: "#2ECC71"

        Rectangle {
            id: indi
            height: parent.height
            radius: parent.radius
            width: parent.width * root.level / 100.0
            color: {
                if (root.level < 10) {
                    return backRect.warningColor
                } else if (root.level < 30) {
                    return backRect.lowColor
                } else if (root.level < 50) {
                    return backRect.midColor
                } else {
                    return backRect.goodColor
                }
            }

            Behavior on width {
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
        }

        Text {
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.bottomMargin: 10
            anchors.leftMargin: 10
            color: "white"
            font.family: "黑体"
            font.pixelSize: 16
            font.bold: true
            text: (charging ? "充电中 " : "剩余电量 ") + root.level + "%"
        }
    }
}
