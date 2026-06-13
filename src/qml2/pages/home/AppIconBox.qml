import QtQuick
import FluentUI

Rectangle {
    id: appIcon
    property string source: ""
    property string title: ""
    property color accent: FluTheme.primaryColor
    readonly property string imageSource: {
        if (source.length === 0)
            return ""
        if (source.indexOf("data:") === 0 || source.indexOf("qrc:") === 0 || source.indexOf("file:") === 0 || source.indexOf("http") === 0)
            return source
        return "data:image/png;base64," + source
    }

    radius: 8
    color: Qt.rgba(accent.r, accent.g, accent.b, FluTheme.dark ? 0.18 : 0.10)
    border.width: 1
    border.color: Qt.rgba(accent.r, accent.g, accent.b, FluTheme.dark ? 0.26 : 0.18)
    clip: true

    Image {
        anchors.fill: parent
        anchors.margins: 3
        source: appIcon.imageSource
        fillMode: Image.PreserveAspectFit
        visible: appIcon.imageSource.length > 0
        asynchronous: true
    }
    FluText {
        anchors.centerIn: parent
        text: appIcon.title.length > 0 ? appIcon.title.charAt(0).toUpperCase() : "?"
        font: FluTextStyle.BodyStrong
        visible: appIcon.imageSource.length === 0
        color: appIcon.accent
    }
}
