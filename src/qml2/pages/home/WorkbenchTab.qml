import QtQuick
import QtQuick.Layouts
import FluentUI

Rectangle {
    id: tab
    property string label: ""
    property int icon: 0
    property int tabIndex: 0
    property int currentIndex: 0
    property color accent: FluTheme.primaryColor
    readonly property bool selected: currentIndex === tabIndex
    signal activated(int tabIndex)

    Layout.fillWidth: true
    Layout.preferredHeight: 32
    radius: 7
    color: selected
           ? Qt.rgba(accent.r, accent.g, accent.b, FluTheme.dark ? 0.24 : 0.14)
           : (mouse.containsMouse ? (FluTheme.dark ? Qt.rgba(1,1,1,0.060) : Qt.rgba(0,0,0,0.036)) : "transparent")
    border.width: selected || mouse.containsMouse ? 1 : 0
    border.color: selected ? Qt.rgba(accent.r, accent.g, accent.b, 0.42) : FluTheme.dividerColor

    RowLayout {
        anchors.centerIn: parent
        spacing: 5
        FluIcon { iconSource: tab.icon; iconSize: 14; iconColor: tab.selected ? tab.accent : FluTheme.fontSecondaryColor }
        FluText { text: tab.label; font: FluTextStyle.Caption; color: tab.selected ? FluTheme.fontPrimaryColor : FluTheme.fontSecondaryColor }
    }

    MouseArea {
        id: mouse
        anchors.fill: parent
        hoverEnabled: true
        onClicked: tab.activated(tab.tabIndex)
    }
}
