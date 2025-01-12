import QtQuick
import QtQuick.Controls

QtObject {
    property color suggestBackColor: Qt.rgba(0, 125 / 255, 200 / 255, 0.1)
    property color suggestHoverColor: Qt.rgba(0, 125 / 255, 200 / 255, 0.3)
    property color suggestClickedColor: Qt.rgba(0, 125 / 255, 200 / 255, 0.8)
    property color ordinaryBackColor: Qt.rgba(0, 0, 0, 0.1)
    property color ordinaryHoverColor: Qt.rgba(0, 0, 0, 0.3)
    property color ordinaryClickedColor: Qt.rgba(0, 0, 0, 0.8)
    property color warningBackColor: Qt.rgba(215 / 255, 80 / 255, 80 / 255, 0.1)
    property color warningHoverColor: Qt.rgba(215 / 255, 80 / 255, 80 / 255, 0.3)
    property color warningClickedColor: Qt.rgba(215 / 255, 80 / 255, 80 / 255, 0.8)

    property color lightTextColor: ordinaryClickedColor
    property color darkTextColor: Qt.rgba(1, 1, 1, 0.8)
}
