import QtQuick
import QtQuick.Layouts
import FluentUI

RowLayout {
    id: pill
    property string label: ""
    property string value: ""
    property color accent: FluTheme.primaryColor
    spacing: 4

    Rectangle { Layout.preferredWidth: 6; Layout.preferredHeight: 6; radius: 3; color: pill.accent }
    FluText { text: pill.label + " " + pill.value; font: FluTextStyle.Caption; color: FluTheme.fontSecondaryColor }
}
