import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import MFloat

MWrapper {
    Layout.preferredHeight: 200
    Layout.fillWidth: true
    title: "电池电量"
    titleRightContent: MSwitchButton {
    }
    Rectangle {
        radius: 12
        border.width: 1
        border.color: "gray"
        color: "transparent"
    }
}