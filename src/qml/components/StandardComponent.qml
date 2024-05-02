import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import MFloat

MFrame {
    property string title: ""
    property string description: ""
    property Component contentItem: null
    property int titleSpace: 40
    wrapperColor: Qt.rgba(255, 255, 255, 0.65)

    ColumnLayout {
        width: parent.width
        height: parent.titleSpace
        anchors.topMargin: 5
        anchors.top: parent.top
        Text {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter
            text: title
            font.bold: true
            font.pixelSize: 16
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
        Text {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter
            text: description
            font.pixelSize: 10
            color: "gray"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }
}
