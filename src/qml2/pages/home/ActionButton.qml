import QtQuick
import QtQuick.Layouts
import FluentUI

Rectangle {
    id: button
    property string label: ""
    property int icon: 0
    property color accent: FluTheme.primaryColor
    property bool dense: false
    signal pressed()

    Layout.preferredHeight: dense ? 30 : 34
    radius: 7
    color: enabled
           ? (mouse.containsMouse ? Qt.rgba(accent.r, accent.g, accent.b, FluTheme.dark ? 0.24 : 0.14)
                                  : (FluTheme.dark ? Qt.rgba(1, 1, 1, 0.055) : Qt.rgba(0, 0, 0, 0.035)))
           : Qt.rgba(0.5, 0.5, 0.5, 0.08)
    border.width: 1
    border.color: mouse.containsMouse ? accent : (FluTheme.dark ? Qt.rgba(1,1,1,0.11) : Qt.rgba(0,0,0,0.10))
    opacity: enabled ? 1 : 0.42

    Behavior on color { ColorAnimation { duration: 120 } }
    Behavior on border.color { ColorAnimation { duration: 120 } }

    RowLayout {
        anchors.centerIn: parent
        spacing: 5
        FluIcon { iconSource: button.icon; iconSize: button.dense ? 13 : 15; iconColor: button.accent; visible: button.icon > 0 }
        FluText { text: button.label; font: button.dense ? FluTextStyle.Caption : FluTextStyle.Body; color: FluTheme.fontPrimaryColor }
    }

    MouseArea {
        id: mouse
        anchors.fill: parent
        hoverEnabled: true
        enabled: button.enabled
        onClicked: button.pressed()
    }
}
