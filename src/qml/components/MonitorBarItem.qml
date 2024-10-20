import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import MFloat

RowLayout {
    required property string title
    required property string valueStr
    property alias value: bar.value
    Layout.fillWidth: true
    Text {
        font.bold: true
        font.pixelSize: 12
        text: title
    }

    MProgressBar {
        id: bar
    }

    Text {
        font.pixelSize: 12
        text: valueStr
    }
}
