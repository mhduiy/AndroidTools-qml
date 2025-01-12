import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import OtherSettingsHandler
import App

Control {
    id: control
    property real alpha: OtherSettingsHandler.wrapperOpacity
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
                        color: App.windowTextColor
                        text: control.title
                        font.bold: true
                        font.pixelSize: 18
                        Behavior on color {
                            ColorAnimation{
                                duration: 200
                            }
                        }
                    }
                }

                Loader {
                    active: description !== ""
                    visible: active
                    Layout.alignment: Qt.AlignTop
                    sourceComponent: Text {
                        color: App.windowTextAssitColor
                        text: control.description
                        font.pixelSize: 12
                        Behavior on color {
                            ColorAnimation{
                                duration: 200
                            }
                        }
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
        color: Qt.rgba(255, 255, 255, control.alpha)
        border.color: App.borderColor
        border.width: 1
        Behavior on color {
            ColorAnimation{
                duration: 200
            }
        }
        Behavior on border.color {
            ColorAnimation{
                duration: 200
            }
        }
    }
}
