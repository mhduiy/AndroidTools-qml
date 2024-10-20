import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

RowLayout {
    required property string title
    required property string content
    property string icon
    Layout.fillWidth: true
    RowLayout {
        spacing: 10
        Rectangle {
            width: 24
            height: 24
            radius: width / 4
            border.color: "gray"
        }
        Text {
            text: title
            opacity: 0.6
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }

    Item {
        Layout.preferredWidth: 30
    }

    Text {
        Layout.fillWidth: true
        text: {
            var text;
            text = content
            return text
        }
        font.pixelSize: 14
        font.family: "黑体"
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignTop
    }
}
