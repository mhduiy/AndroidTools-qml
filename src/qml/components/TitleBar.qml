import QtQuick
import QtQuick.Layouts

Rectangle {
    id: titleBar
    color: "transparent"
    signal requestCloseWindow()
    signal requestMinWindow()
    signal requestMoveWindow(int tarX, int tarY)

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        Text {
            text: "AndroidTools"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 14
        }

        Item {
            Layout.fillWidth: true
        }

        Rectangle {
            Layout.preferredWidth: 30
            Layout.fillHeight: true
            color: "transparent"
            Canvas {
                anchors.fill: parent
                onPaint: {
                    var ctx = getContext("2d");
                    ctx.strokeStyle = colorConstants.ordinaryClickedColor
                    ctx.lineWidth = 4;
                    ctx.beginPath();
                    ctx.moveTo(10, 15);
                    ctx.lineTo(20, 15);
                    ctx.stroke();
                }
            }
            Behavior on scale {
                NumberAnimation {
                    duration: 300
                    easing.type: Easing.OutBack
                }
            }
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onEntered: {
                    parent.scale = 1.5
                }
                onExited:  {
                    parent.scale = 1.0
                }
                onClicked: {
                    requestMinWindow()
                }

            }
        }
        Rectangle {
            Layout.preferredWidth: 30
            Layout.fillHeight: true
            color: "transparent"
            Canvas {
                anchors.fill: parent
                onPaint: {
                    var ctx = getContext("2d");
                    ctx.strokeStyle = colorConstants.warningClickedColor
                    ctx.lineWidth = 4;
                    ctx.beginPath();
                    ctx.moveTo(10, 20);
                    ctx.lineTo(20, 10);
                    ctx.moveTo(10, 10);
                    ctx.lineTo(20, 20);
                    ctx.stroke();
                }
            }
            Behavior on scale {
                NumberAnimation {
                    duration: 300
                    easing.type: Easing.OutBack
                }
            }
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onEntered: {
                    parent.scale = 1.5
                }
                onExited:  {
                    parent.scale = 1.0
                }

                onClicked: {
                    requestCloseWindow()
                }
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        property bool dragging: false
        property int dragX: 0
        property int dragY: 0
        propagateComposedEvents: true
        onPressed: {
            dragX = mouseX
            dragY = mouseY
            dragging = true
        }
        onReleased: {
            dragging = false
        }
        onPositionChanged: {
            if (dragging) {
                var dragGlobalPoint = mapToGlobal(mouseX - dragX, mouseY - dragY)
                requestMoveWindow(dragGlobalPoint.x, dragGlobalPoint.y)
            }
        }
    }
}
