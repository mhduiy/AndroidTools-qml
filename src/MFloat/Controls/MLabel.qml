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

    Rectangle {
        id: rect
        anchors.fill: parent
        color: root.rectColor
        radius: root.radius
        Text {
            x: hMargin
            y: vMargin
            id: centerText
            text: root.text
            color: root.textColor
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }
}
