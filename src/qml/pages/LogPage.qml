
import QtQuick
import MFloat
import QtQuick.Controls
import QtQuick.Layouts

import "../components/"
import ADBLog 1.0
import App 1.0


ItemPage {

    RowLayout {
        anchors.fill: parent
       MWrapper {
            Layout.fillWidth: true
            Layout.fillHeight: true
            title: "日志"
            description: "包含程序执行的所有ADB命令的日志记录"

            ListView {
                spacing: 2
                clip: true
                model: ADBLog
                onCountChanged: {
        positionViewAtEnd()
    }
                delegate: Item {
                    implicitHeight: label.height + 4
                    implicitWidth: ListView.view.width
                    Rectangle {
                        anchors.fill: parent
                        border.width: 1
                        color: hoverHandler.hovered ? (model.type === 0 ? App.normalColorWeight1 : App.warningColorWeight1) : "transparent"
                        border.color: hoverHandler.hovered ? (model.type === 0 ? App.normalColorWeight3 : App.warningColorWeight3) : "transparent"
                        Label {
                            anchors.left: parent.left
                            anchors.leftMargin: 2
                            anchors.verticalCenter: parent.verticalCenter
                            id: label
                            text: model.log
                            color: model.type === 0 ? App.normalColorWeight3 : App.warningColorWeight3
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                    HoverHandler {
                        id: hoverHandler
                    }
                }
            }
        }
    }
}