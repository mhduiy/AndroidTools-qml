import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import MFloat

ListView {
    id: listView
    clip: true
    currentIndex: 0
    height: contentHeight
    model: [
        "设备信息",
        "设备控制",
        "软件管理",
        "刷机工具",
        "设备镜像",
        "ADB日志",
        "设置",
    ]
    delegate: lvDelegate
    highlightMoveDuration: 300
    focus: true
    highlight: Rectangle {
        color: Qt.rgba(135 / 255, 206 / 255, 250 / 255, 0.6)
        radius: 6
    }
    spacing: 10

    Component {
        id: lvDelegate
        Rectangle {
            id: wrapper
            width: ListView.view.width
            height: 40
            radius: 6
            color: Qt.rgba(255,255,255, ListView.isCurrentItem ? 0 : 0.3)
            Behavior on color {
                PropertyAnimation {
                    duration: 300
                }
            }
            Text {
                anchors.centerIn: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: modelData
                font.pixelSize: 16
                font.bold: true
                font.family: "黑体"
            }

            Rectangle {
                anchors.left: parent.left
                anchors.leftMargin: 10
                scale: wrapper.ListView.isCurrentItem ? 1.0 : 0
                anchors.verticalCenter: parent.verticalCenter
                height: wrapper.height * 0.6
                width: 5
                color: "#008c8c"
                radius: 4

                Behavior on scale {
                    PropertyAnimation {
                        duration: 400
                        easing.type: Easing.OutBack
                    }
                }
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    listView.currentIndex = index
                }
            }
        }

    }
}
