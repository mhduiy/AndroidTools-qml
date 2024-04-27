import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import MFloat

Item {
    id: root
    Layout.preferredWidth: parent.width
    Layout.fillWidth: true
    property alias title: titleLabel.text
    property alias description: desLabel.text
    property Component controlItem: null
    property Component expandItem: null
    property int mainLayoutHeight: 60
    property int expandLayoutHeight: 100
    property alias expendVisible: expendItem.visible
    height: mainLayoutHeight + (expendItem.visible ? expandLayoutHeight : 0)
    MFrame {
        anchors.fill: parent
        wrapperColor: Qt.rgba(255, 255, 255, 0.65)
        ColumnLayout {
            anchors.fill: parent
            RowLayout {
                id: mainControlLayout
                height: 60
                Item {
                    Layout.alignment: Qt.AlignVCenter
                    Layout.leftMargin: 10
                    height: titleLabel.height + desLabel.height
                    width: 100
                    Column {
                        Label {
                            id: titleLabel
                            font.pixelSize: 16
                        }
                        Label {
                            id: desLabel
                            font.pixelSize: 12
                            color: "gray"
                        }
                    }
                }

                Item {
                    Layout.fillWidth: true
                }

                Item {
                    Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
                    Layout.fillHeight: true
                    Layout.rightMargin: 10
                    width: 50
                    Loader {
                        anchors.verticalCenter: parent.verticalCenter
                        sourceComponent: controlItem
                    }
                }
            }
            Rectangle {
                id: expendItem
                visible: false
                Loader {
                    sourceComponent: expandItem
                }
            }
        }
    }
}
