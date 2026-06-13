import QtQuick
import QtQuick.Layouts
import FluentUI

Rectangle {
    id: card

    property string label: ""
    property string value: "--"
    property string unit: ""
    property string detail: ""
    property color accent: FluTheme.primaryColor
    property real level: 0

    Layout.fillWidth: true
    Layout.preferredHeight: 78
    radius: 8
    color: FluTheme.dark ? Qt.rgba(0.08, 0.09, 0.11, 0.76) : Qt.rgba(1, 1, 1, 0.78)
    border.width: 1
    border.color: Qt.rgba(accent.r, accent.g, accent.b, FluTheme.dark ? 0.24 : 0.16)

    Rectangle {
        width: 4
        radius: 2
        anchors { left: parent.left; top: parent.top; bottom: parent.bottom; margins: 8 }
        color: card.accent
    }

    ColumnLayout {
        anchors { fill: parent; leftMargin: 18; rightMargin: 12; topMargin: 8; bottomMargin: 8 }
        spacing: 4

        RowLayout {
            Layout.fillWidth: true
            FluText {
                text: card.label
                font: FluTextStyle.Caption
                color: FluTheme.fontSecondaryColor
                Layout.fillWidth: true
                elide: Text.ElideRight
            }
            Rectangle {
                Layout.preferredWidth: 7
                Layout.preferredHeight: 7
                radius: 4
                color: card.accent
            }
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 4
            FluText {
                text: card.value
                font.pixelSize: 23
                font.bold: true
                color: FluTheme.fontPrimaryColor
                Layout.fillWidth: true
                elide: Text.ElideRight
            }
            FluText {
                text: card.unit
                font: FluTextStyle.Caption
                color: FluTheme.fontSecondaryColor
                visible: card.unit.length > 0
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 5
            radius: 3
            color: FluTheme.dark ? Qt.rgba(1, 1, 1, 0.08) : Qt.rgba(0, 0, 0, 0.055)
            Rectangle {
                height: parent.height
                radius: parent.radius
                width: Math.max(0, Math.min(parent.width, parent.width * card.level / 100))
                color: card.accent
                Behavior on width { NumberAnimation { duration: 260; easing.type: Easing.OutCubic } }
            }
        }

        FluText {
            text: card.detail
            font: FluTextStyle.Caption
            color: FluTheme.fontSecondaryColor
            Layout.fillWidth: true
            elide: Text.ElideRight
        }
    }
}
