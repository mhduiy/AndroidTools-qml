import QtQuick
import QtQuick.Layouts
import FluentUI
import NotificationController 1.0

Rectangle {
    id: notif
    property string titleText: ""
    property string contentText: ""
    property int notifType: 0
    property int displayDuration: 3000

    width: 320
    height: Math.max(56, col.implicitHeight + 24)
    radius: 8
    color: FluTheme.dark ? Qt.rgba(0.1, 0.1, 0.12, 0.96) : Qt.rgba(0.98, 0.98, 0.98, 0.96)
    border.width: 1
    border.color: notifType === NotificationController.Error ? "#e74c3c" : (notifType === NotificationController.Warning ? "#e67e22" : FluTheme.primaryColor)

    anchors {
        right: parent ? parent.right : undefined
        rightMargin: parent ? 12 : 0
        top: parent ? parent.top : undefined
        topMargin: 12
    }
    x: parent ? parent.width + 20 : 0

    Behavior on x {
        NumberAnimation { duration: 350; easing.type: Easing.OutCubic }
    }

    ColumnLayout {
        id: col
        anchors { left: parent.left; right: parent.right; top: parent.top; margins: 12 }
        spacing: 2

        FluText {
            text: notif.titleText
            font: FluTextStyle.BodyStrong
            color: notifType === NotificationController.Error ? "#e74c3c" : (notifType === NotificationController.Warning ? "#e67e22" : FluTheme.fontPrimaryColor)
            Layout.fillWidth: true
        }
        FluText {
            text: notif.contentText
            font: FluTextStyle.Caption
            color: FluTheme.fontSecondaryColor
            Layout.fillWidth: true
            wrapMode: Text.WordWrap
        }
    }

    MouseArea { anchors.fill: parent; onClicked: dismiss() }

    Timer {
        id: dismissTimer
        interval: displayDuration
        onTriggered: dismiss()
    }

    function show() {
        x = parent.width - width - 12
        dismissTimer.restart()
    }

    function dismiss() {
        x = parent.width + 20
        dismissTimer.stop()
        Qt.callLater(function() { notif.destroy() })
    }
}
