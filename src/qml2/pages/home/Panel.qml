import QtQuick
import FluentUI

Rectangle {
    id: panel
    default property alias content: body.data

    radius: 8
    color: FluTheme.dark ? Qt.rgba(0.10, 0.11, 0.13, 0.76) : Qt.rgba(1, 1, 1, 0.78)
    border.width: 1
    border.color: FluTheme.dark ? Qt.rgba(1, 1, 1, 0.10) : Qt.rgba(0, 0, 0, 0.11)
    layer.enabled: true

    Item {
        id: body
        anchors.fill: parent
    }
}
