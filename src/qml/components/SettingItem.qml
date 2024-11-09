import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import MFloat

MWrapper {
    id: root
    property var title
    property var description
    property Component controlItem: null
    property int mainLayoutHeight: 60
    Layout.fillWidth: true
    RowLayout {
        id: mainControlLayout
        ColumnLayout {
            Label {
                id: titleLabel
                font.pixelSize: 16
                text: root.title
            }
            Label {
                id: desLabel
                font.pixelSize: 12
                color: "gray"
                text: root.description
            }
        }

        Item {
            Layout.fillWidth: true
        }

        Loader {
            sourceComponent: controlItem
        }
    }
}
