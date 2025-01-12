import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import MFloat
import App

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
            Text {
                id: titleLabel
                font.pixelSize: 16
                color: App.windowTextColor
                text: root.title
                Behavior on color {
                    ColorAnimation{
                        duration: 200
                    }
                }
            }
            Text {
                id: desLabel
                font.pixelSize: 12
                color: App.windowTextAssitColor
                text: root.description
                Behavior on color {
                    ColorAnimation{
                        duration: 200
                    }
                }
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
