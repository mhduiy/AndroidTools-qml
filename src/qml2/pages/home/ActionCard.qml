import QtQuick
import QtQuick.Layouts
import FluentUI

Rectangle {
    id: action
    property string title: ""
    property string subtitle: ""
    property int icon: 0
    property color accent: FluTheme.primaryColor
    signal pressed()

    Layout.fillWidth: true
    Layout.preferredHeight: 48
    radius: 7
    color: mouse.containsMouse ? Qt.rgba(accent.r, accent.g, accent.b, FluTheme.dark ? 0.18 : 0.10)
                               : (FluTheme.dark ? Qt.rgba(1, 1, 1, 0.04) : Qt.rgba(0, 0, 0, 0.024))
    border.width: 1
    border.color: mouse.containsMouse ? accent : Qt.rgba(accent.r, accent.g, accent.b, FluTheme.dark ? 0.20 : 0.14)
    opacity: enabled ? 1 : 0.42

    RowLayout {
        anchors { fill: parent; margins: 8 }
        spacing: 8
        FluIcon { iconSource: action.icon; iconSize: 17; iconColor: action.accent; Layout.preferredWidth: 20 }
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 0
            FluText { text: action.title; font: FluTextStyle.Caption; Layout.fillWidth: true; elide: Text.ElideRight }
            FluText { text: action.subtitle; font: FluTextStyle.Caption; color: FluTheme.fontSecondaryColor; Layout.fillWidth: true; elide: Text.ElideRight }
        }
    }

    MouseArea {
        id: mouse
        anchors.fill: parent
        hoverEnabled: true
        enabled: action.enabled
        onClicked: action.pressed()
    }
}
