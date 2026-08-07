import QtQuick
import FluentUI
import OtherSettingsHandler 1.0

Rectangle {
    id: panel
    default property alias content: body.data

    radius: 8
    color: FluTheme.dark ? Qt.rgba(0.10, 0.11, 0.13, OtherSettingsHandler.wrapperOpacity) : Qt.rgba(1, 1, 1, OtherSettingsHandler.wrapperOpacity)
    border.width: 1
    border.color: FluTheme.dark ? Qt.rgba(1, 1, 1, 0.10) : Qt.rgba(0, 0, 0, 0.11)
    layer.enabled: true

    Item {
        id: body
        anchors.fill: parent
    }
}
