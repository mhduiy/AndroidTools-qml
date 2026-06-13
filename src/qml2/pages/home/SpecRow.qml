import QtQuick
import QtQuick.Layouts
import FluentUI

RowLayout {
    id: row
    property string name: ""
    property string value: ""

    spacing: 8
    FluText { text: row.name; font: FluTextStyle.Caption; color: FluTheme.fontSecondaryColor; Layout.preferredWidth: 48 }
    FluText { text: row.value; font: FluTextStyle.BodyStrong; elide: Text.ElideRight; Layout.fillWidth: true }
}
