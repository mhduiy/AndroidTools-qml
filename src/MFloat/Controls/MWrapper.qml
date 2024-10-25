import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Control {
    id: control
    property real alpha: 0.65
    property string title
    property string description
    default property Component content
    property Component titleRightContent

    padding: 10

    contentItem: ColumnLayout {
        id: layout
        Layout.alignment: Qt.AlignTop

        RowLayout {
            visible: title != "" || description != ""
            ColumnLayout {
                Loader {
                    active: title !== ""
                    visible: active
                    Layout.alignment: Qt.AlignTop
                    sourceComponent: Text {
                        text: control.title
                        font.bold: true
                        font.pixelSize: 18
                    }
                }

                Loader {
                    active: description !== ""
                    visible: active
                    Layout.alignment: Qt.AlignTop
                    sourceComponent: Text {
                        text: control.description
                        font.pixelSize: 12
                        opacity: 0.6
                    }
                }
            }
            Item {
                Layout.fillWidth: true
            }

            Loader {
                sourceComponent: control.titleRightContent
            }
        }

        Loader {
            Layout.alignment: Qt.AlignTop
            Layout.fillWidth: true
            Layout.fillHeight: true
            sourceComponent: control.content
        }
    }

    background: Rectangle {
        radius: 10
        color: Qt.rgba(255, 255, 255, alpha)
        border.color: colorConstants.ordinaryHoverColor
        border.width: 1
    }
}
