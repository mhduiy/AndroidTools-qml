import QtQuick

Item {
    id: root
    width: 20
    height: 20

    property int checkState: Qt.Unchecked
    property bool tristate: false

    ColorConstants {
        id: colorConstants
    }

    Rectangle {
        id: wrapperRect
        anchors.fill: parent
        border.width: 2
        radius: 3
        color: colorConstants.suggestBackColor
        border.color: colorConstants.suggestClickedColor

        Rectangle {
            id: checkRect
            anchors.fill: parent
            color: "transparent"
            Canvas {
                id: canvas
                anchors.fill: parent
                onPaint: {
                    var ctx = getContext("2d");
                    if (checkState === Qt.Checked) {
                        ctx.strokeStyle = colorConstants.suggestClickedColor
                        ctx.lineWidth = 2;
                        ctx.beginPath();
                        ctx.moveTo(3, 10);
                        ctx.lineTo(8, 15);
                        ctx.lineTo(17, 5);
                        ctx.stroke();
                    }
                }
            }

            Behavior on scale {
                PropertyAnimation {
                    duration: 300
                    easing.type: Easing.OutQuint
                }
            }

            states: [
                State {
                    name: "visible"
                    PropertyChanges { target: checkRect; scale: 1.0 } // 当 visible 变为 true 时，将 scale 设置为 1.0
                },
                State {
                    name: "hidden"
                    PropertyChanges { target: checkRect; scale: 0 } // 当 visible 变为 false 时，将 scale 设置为 0
                }
            ]

            Component.onCompleted: {
                visible = checkState === Qt.Checked
                scale = checkState === Qt.Checked ? 1.0 : 0
            }
        }

        Rectangle {
            id: partiallyCheckRect
            anchors.fill: parent
            color: "transparent"
            Rectangle {
                anchors.centerIn: parent
                width: 10
                height: 10
                x: 3
                color: colorConstants.suggestClickedColor
                radius: height / 10
            }

            Behavior on scale {
                PropertyAnimation {
                    duration: 300
                    easing.type: Easing.OutQuint
                }
            }

            states: [
                State {
                    name: "visible"
                    PropertyChanges { target: partiallyCheckRect; scale: 1.0 } // 当 visible 变为 true 时，将 scale 设置为 1.0
                },
                State {
                    name: "hidden"
                    PropertyChanges { target: partiallyCheckRect; scale: 0 } // 当 visible 变为 false 时，将 scale 设置为 0
                }
            ]

            Component.onCompleted: {
                visible = checkState === Qt.PartiallyChecked
                scale = checkState === Qt.PartiallyChecked ? 1.0 : 0
            }
        }


        MouseArea {
            anchors.fill: parent
            onClicked: {
                checkRect.visible = true
                partiallyCheckRect.visible = true
                if (checkState === Qt.Unchecked) {
                    checkState = Qt.Checked;
                    canvas.requestPaint()
                    checkRect.state = "visible"
                } else if (checkState === Qt.Checked && tristate) {
                    checkState = Qt.PartiallyChecked;
                    checkRect.state = "hidden"
                    partiallyCheckRect.state = "visible"
                } else {
                    checkState = Qt.Unchecked;
                    checkRect.state = "hidden"
                    partiallyCheckRect.state = "hidden"
                }
            }
        }
    }
}
