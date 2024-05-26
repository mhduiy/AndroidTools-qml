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
    property int mainLayoutHeight: 60
    property int controlItemWidth: 30
    height: mainLayoutHeight
    MFrame {
        anchors.fill: parent
        wrapperColor: Qt.rgba(255, 255, 255, 0.65)
        RowLayout {
            anchors.fill: parent
            id: mainControlLayout
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
                Layout.preferredWidth: root.controlItemWidth
                Layout.maximumWidth: root.controlItemWidth
                Layout.minimumWidth: root.controlItemWidth
                Loader {
                    anchors.verticalCenter: parent.verticalCenter
                    sourceComponent: controlItem
                }
            }
        }
    }
}
