import QtQuick

Item {
    id: root
    width: 200
    height: 30

    property int value: 50
    property bool showText: false

    ColorConstants {
        id: colorConstants
    }

    Rectangle {
        anchors.fill: parent
        color: colorConstants.suggestBackColor
        radius: 10
        border.width: 2
        border.color: colorConstants.suggestClickedColor

        Rectangle {
            id: thumb
            x: parent.border.width + 1
            y: parent.border.width + 1
            radius: 10 * height / parent.height
            height: parent.height - parent.border.width * 2 - 2
            width: (parent.width -  parent.border.width * 2 - 2) * value / 100
            color: colorConstants.suggestClickedColor

            Behavior on width {
                NumberAnimation{
                    duration: 200
                    easing.type: Easing.OutCubic
                }
            }
        }

        Text {
            anchors.fill: parent
            id: text
            text: value + "%"
            visible: showText
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }
}
