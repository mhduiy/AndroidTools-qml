import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import MFloat

MFrame {
    property string title: ""
    property string description: ""
    property Component contentItem: null
    wrapperColor: Qt.rgba(255, 255, 255, 0.65)

    ColumnLayout {
        width: parent.width - 20
        height: parent.height - 20
        anchors.centerIn: parent
        Text {
            Layout.alignment: Qt.AlignHCenter
            text: title
            font.bold: true
            font.pixelSize: 16
        }
        Text {
            Layout.alignment: Qt.AlignHCenter
            text: description
            font.pixelSize: 10
            color: "gray"
        }
        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Loader {
                sourceComponent: contentItem
            }
        }
    }


}
