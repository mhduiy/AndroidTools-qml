import QtQuick
import QtQuick.Layouts
import FluentUI

Rectangle {
    id: key
    property string label: ""
    property int icon: 0
    property color accent: FluTheme.primaryColor
    signal pressed()

    Layout.preferredWidth: 62
    Layout.preferredHeight: 38
    radius: 8
    color: mouse.containsMouse ? Qt.rgba(accent.r, accent.g, accent.b, FluTheme.dark ? 0.25 : 0.14)
                               : (FluTheme.dark ? Qt.rgba(1,1,1,0.045) : Qt.rgba(0,0,0,0.030))
    border.color: mouse.containsMouse ? accent : (FluTheme.dark ? Qt.rgba(1,1,1,0.10) : Qt.rgba(0,0,0,0.09))
    border.width: 1
    opacity: enabled ? 1 : 0.4

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 2
        FluIcon { iconSource: key.icon; iconSize: 16; iconColor: key.accent; Layout.alignment: Qt.AlignHCenter }
        FluText { text: key.label; font: FluTextStyle.Caption; Layout.alignment: Qt.AlignHCenter }
    }

    MouseArea {
        id: mouse
        anchors.fill: parent
        hoverEnabled: true
        enabled: key.enabled
        onClicked: key.pressed()
    }
}
