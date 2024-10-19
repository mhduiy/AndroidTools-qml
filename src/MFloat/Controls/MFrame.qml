import QtQuick

Rectangle {
    id: root
    width: 10
    height: 10
    property color wrapperColor: colorConstants.ordinaryBackColor

    ColorConstants {
        id: colorConstants
    }

    color: wrapperColor
    border.width: 1
    radius: 10
    border.color: colorConstants.ordinaryHoverColor
    opacity: 1
}
