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
    property int stackIndex: 0
    property real targetY: 18
    property real progress: 1
    property bool closing: false
    readonly property color accentColor: notifType === NotificationController.Error ? "#d83b01"
                                       : notifType === NotificationController.Warning ? "#ca8a04"
                                       : FluTheme.primaryColor
    readonly property color softAccent: Qt.rgba(accentColor.r, accentColor.g, accentColor.b, FluTheme.dark ? 0.20 : 0.12)
    readonly property real targetX: parent ? parent.width - width - 18 : 0
    readonly property real hiddenX: parent ? parent.width + 36 : 0

    signal closed(var item)

    width: parent ? Math.min(380, Math.max(292, parent.width - 36)) : 360
    height: Math.max(76, contentColumn.implicitHeight + 26)
    x: hiddenX
    y: targetY
    z: 1000 - stackIndex
    scale: 0.94
    opacity: 0
    radius: 10
    antialiasing: true
    color: FluTheme.dark ? Qt.rgba(0.08, 0.09, 0.11, 0.96) : Qt.rgba(1, 1, 1, 0.96)
    border.width: 1
    border.color: Qt.rgba(accentColor.r, accentColor.g, accentColor.b, FluTheme.dark ? 0.34 : 0.22)
    layer.enabled: true

    Behavior on y {
        NumberAnimation { duration: 340; easing.type: Easing.OutCubic }
    }

    Behavior on scale {
        NumberAnimation { duration: 220; easing.type: Easing.OutCubic }
    }

    Rectangle {
        anchors.fill: parent
        radius: parent.radius
        opacity: FluTheme.dark ? 0.35 : 0.55
        gradient: Gradient {
            GradientStop { position: 0.0; color: notif.softAccent }
            GradientStop { position: 0.42; color: "transparent" }
            GradientStop { position: 1.0; color: FluTheme.dark ? Qt.rgba(1,1,1,0.030) : Qt.rgba(0,0,0,0.018) }
        }
    }

    Rectangle {
        width: 4
        radius: 2
        anchors { left: parent.left; top: parent.top; bottom: parent.bottom; margins: 8 }
        color: notif.accentColor
    }

    Rectangle {
        id: glow
        width: 88
        height: parent.height * 2
        y: -parent.height / 2
        x: -width - 26
        rotation: 18
        opacity: FluTheme.dark ? 0.16 : 0.28
        gradient: Gradient {
            orientation: Gradient.Horizontal
            GradientStop { position: 0.0; color: "transparent" }
            GradientStop { position: 0.5; color: "#ffffff" }
            GradientStop { position: 1.0; color: "transparent" }
        }
    }

    RowLayout {
        id: contentColumn
        anchors { left: parent.left; right: parent.right; top: parent.top; margins: 14 }
        anchors.leftMargin: 16
        anchors.rightMargin: 42
        spacing: 10

        Rectangle {
            Layout.preferredWidth: 30
            Layout.preferredHeight: 30
            Layout.alignment: Qt.AlignTop
            radius: 15
            color: Qt.rgba(notif.accentColor.r, notif.accentColor.g, notif.accentColor.b, FluTheme.dark ? 0.24 : 0.13)
            border.width: 1
            border.color: Qt.rgba(notif.accentColor.r, notif.accentColor.g, notif.accentColor.b, 0.38)

            FluText {
                anchors.centerIn: parent
                text: notif.notifType === NotificationController.Error ? "x"
                      : notif.notifType === NotificationController.Warning ? "!"
                      : "i"
                font: FluTextStyle.BodyStrong
                color: notif.accentColor
            }
        }

        ColumnLayout {
            Layout.fillWidth: true
            spacing: 3

            FluText {
                text: notif.titleText
                font: FluTextStyle.BodyStrong
                color: FluTheme.fontPrimaryColor
                Layout.fillWidth: true
                elide: Text.ElideRight
            }

            FluText {
                text: notif.contentText
                font: FluTextStyle.Caption
                color: FluTheme.fontSecondaryColor
                Layout.fillWidth: true
                wrapMode: Text.WordWrap
                maximumLineCount: 3
                elide: Text.ElideRight
            }
        }
    }

    Rectangle {
        id: closeButton
        width: 26
        height: 26
        radius: 7
        anchors { right: parent.right; top: parent.top; margins: 10 }
        color: closeMouse.containsMouse ? Qt.rgba(notif.accentColor.r, notif.accentColor.g, notif.accentColor.b, FluTheme.dark ? 0.22 : 0.12) : "transparent"

        FluText {
            anchors.centerIn: parent
            text: "x"
            font: FluTextStyle.Caption
            color: FluTheme.fontSecondaryColor
        }

        MouseArea {
            id: closeMouse
            anchors.fill: parent
            hoverEnabled: true
            onClicked: notif.dismiss()
        }
    }

    Rectangle {
        anchors { left: parent.left; right: parent.right; bottom: parent.bottom }
        height: 3
        radius: 2
        color: FluTheme.dark ? Qt.rgba(1,1,1,0.08) : Qt.rgba(0,0,0,0.055)

        Rectangle {
            width: parent.width * notif.progress
            height: parent.height
            radius: parent.radius
            color: notif.accentColor
        }
    }

    MouseArea {
        anchors.fill: parent
        anchors.rightMargin: 36
        hoverEnabled: true
        onClicked: notif.dismiss()
    }

    Timer {
        id: dismissTimer
        interval: Math.max(900, notif.displayDuration)
        onTriggered: notif.dismiss()
    }

    ParallelAnimation {
        id: enterAnimation
        NumberAnimation { target: notif; property: "x"; from: notif.hiddenX; to: notif.targetX; duration: 520; easing.type: Easing.OutBack; easing.overshoot: 0.9 }
        NumberAnimation { target: notif; property: "opacity"; from: 0; to: 1; duration: 220; easing.type: Easing.OutCubic }
        NumberAnimation { target: notif; property: "scale"; from: 0.94; to: 1; duration: 420; easing.type: Easing.OutBack; easing.overshoot: 0.55 }
    }

    SequentialAnimation {
        id: glowAnimation
        PauseAnimation { duration: 90 }
        NumberAnimation { target: glow; property: "x"; from: -glow.width - 26; to: notif.width + 26; duration: 720; easing.type: Easing.InOutCubic }
    }

    NumberAnimation {
        id: progressAnimation
        target: notif
        property: "progress"
        from: 1
        to: 0
        duration: Math.max(900, notif.displayDuration)
        easing.type: Easing.Linear
    }

    ParallelAnimation {
        id: exitAnimation
        NumberAnimation { target: notif; property: "x"; to: notif.hiddenX; duration: 260; easing.type: Easing.InCubic }
        NumberAnimation { target: notif; property: "opacity"; to: 0; duration: 180; easing.type: Easing.InCubic }
        NumberAnimation { target: notif; property: "scale"; to: 0.98; duration: 220; easing.type: Easing.InCubic }
        onFinished: notif.closed(notif)
    }

    function show() {
        closing = false
        x = hiddenX
        y = targetY
        opacity = 0
        scale = 0.94
        progress = 1
        enterAnimation.restart()
        glowAnimation.restart()
        progressAnimation.restart()
        dismissTimer.restart()
    }

    function dismiss() {
        if (closing)
            return
        closing = true
        dismissTimer.stop()
        progressAnimation.stop()
        glowAnimation.stop()
        exitAnimation.restart()
    }
}
