import QtQuick

Item {
    id: root
    width: centerText.width + hMargin * 2
    height: centerText.height + vMargin * 2
    property color rectColor: "#008c8c"
    property string text: ""
    property color textColor: "#ffffff"
    property int vMargin: 2
    property int hMargin: 2
    property int radius: 5
    property Text wrapperText: centerText
    property alias font: centerText.font

    Rectangle {
        id: rect
        anchors.fill: parent
        color: root.rectColor
        radius: root.radius
        Text {
            id: centerText
            x: hMargin
            y: vMargin
            text: root.text
            color: root.textColor
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }
}
