import QtQuick

Item {
    id: root
    width: 50
    height: 25

    property bool status: false

    ColorConstants {
        id: colorConstants
    }

    Rectangle {
        anchors.fill: parent
        color: status ? colorConstants.suggestBackColor : colorConstants.ordinaryBackColor
        border.width: 2
        border.color: status ? colorConstants.suggestClickedColor : colorConstants.ordinaryClickedColor
        radius: width / 2

        Behavior on color {
            ColorAnimation{
                duration: 200
            }
        }

        Behavior on border.color {
            ColorAnimation{
                duration: 200
            }
        }

        Rectangle {
            id: thumb
            y: 3
            height: parent.height - 6
            width: height
            radius: width / 2
            scale: 1.0
            color: status ? colorConstants.suggestClickedColor : colorConstants.ordinaryClickedColor

            Component.onCompleted: {
                x = status ? parent.width - width - 3 : 3
            }

            Behavior on color {
                ColorAnimation{
                    duration: 200
                }
            }

            SequentialAnimation {
                id: scaleAni
                NumberAnimation {
                    target: thumb
                    from: 1.0
                    to: 0.6
                    duration: 80
                    property: "scale"
                }

                NumberAnimation{
                    target: thumb
                    from: status ? 3 : thumb.parent.width - thumb.width
                    to: status ? thumb.parent.width - thumb.width - 3 : 3
                    duration: 150
                    property: "x"
                    easing.type: Easing.InOutCirc
                }

                NumberAnimation {
                    target: thumb
                    from: 0.6
                    to: 1.0
                    duration: 80
                    property: "scale"
                }
            }
        }
    }
    MouseArea {
        anchors.fill: parent
        onClicked: {
            if (!scaleAni.running) {
                status = !status
                scaleAni.start()
            }
        }
    }
}
