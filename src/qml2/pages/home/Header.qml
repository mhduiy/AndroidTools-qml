import QtQuick
import QtQuick.Layouts
import FluentUI

RowLayout {
    id: header
    property string title: ""
    property string subtitle: ""

    spacing: 7
    FluText { text: header.title; font: FluTextStyle.BodyStrong }
    FluText {
        text: header.subtitle
        font: FluTextStyle.Caption
        color: FluTheme.fontSecondaryColor
        elide: Text.ElideRight
        Layout.fillWidth: true
    }
}
