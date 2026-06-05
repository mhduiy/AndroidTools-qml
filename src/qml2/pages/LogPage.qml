import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import FluentUI
import ADBLog 1.0

FluContentPage {
    id: page
    title: "ADB日志"

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 8
        spacing: 8

        RowLayout {
            Layout.fillWidth: true
            spacing: 6
            FluTextBox { id: searchBox; Layout.fillWidth: true; placeholderText: "过滤..." }
            FluButton { text: "清除"; onClicked: {} }
            FluToggleSwitch { id: autoScroll; checked: true }
            FluText { text: "自动滚动"; font: FluTextStyle.Caption; color: FluTheme.fontSecondaryColor }
            FluText { text: ADBLog.rowCount() + " 条"; font: FluTextStyle.Caption; color: FluTheme.fontSecondaryColor }
        }

        FluFrame {
            Layout.fillWidth: true
            Layout.fillHeight: true

            ListView {
                id: logList
                anchors { fill: parent; margins: 6 }
                clip: true
                model: ADBLog
                spacing: 1

                delegate: Rectangle {
                    width: ListView.view.width
                    height: Math.max(26, logLabel.implicitHeight + 6)
                    color: index % 2 ? "transparent" : (FluTheme.dark ? Qt.rgba(1,1,1,0.03) : Qt.rgba(0,0,0,0.02))
                    radius: 4

                    FluText {
                        id: logLabel
                        anchors { left: parent.left; right: parent.right; verticalCenter: parent.verticalCenter; margins: 6 }
                        text: model.display || ""
                        font.pixelSize: 12
                        font.family: "monospace"
                        wrapMode: Text.WrapAnywhere
                        color: {
                            var t = model.type || 0
                            if (t === 1) return "#e67e22"
                            if (t === 2) return "#e74c3c"
                            return FluTheme.fontPrimaryColor
                        }
                    }
                }
            }
        }
    }

    Connections {
        target: ADBLog
        function onRowsInserted() { if (autoScroll.checked) logList.positionViewAtEnd() }
    }
}
