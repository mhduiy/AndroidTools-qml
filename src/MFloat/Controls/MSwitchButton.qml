import QtQuick
import App

Item {
    id: root
    width: 50
    height: 25

    property bool status: false

    Rectangle {
        anchors.fill: parent
        color: status ? App.suggestColorWeight1 : App.normalColorWeight1
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
            color: status ? App.suggestColorWeight3 : App.adjustColorOpacity(App.normalColorWeight3, 0.6)

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
                NumberAnimation{
                    target: thumb
                    from: status ? 3 : thumb.parent.width - thumb.width - 3
                    to: status ? thumb.parent.width - thumb.width - 3 : 3
                    duration: 200
                    property: "x"
                    easing.type: Easing.InOutCirc
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
