import QtQuick
import QtQuick.Controls

Item {
    id: root

    enum FBtnType {
        Ordinary = 0,
        Suggest = 1,
        Warning = 2
    }
    enum FBtnState {
        Normal = 0,
        Hover = 1,
        Clicked = 2
    }

    width: 65
    height: 30

    property alias text: label.text
    property alias btnType: btnRect.btnType
    property int fontSize: 12
    signal clicked

    ColorConstants {
        id: colorConstants
    }

    Rectangle {
        id: btnRect
        width: root.width
        height: root.height
        clip: true
        signal pressed
        property int btnType: MButton.FBtnType.Ordinary
        property int btnState: MButton.FBtnState.Normal
        property int mouseClickCenterX: 0
        property int mouseClickCenterY: 0
        property var backgroundColorMap: [
            { key: MButton.FBtnType.Ordinary, value: colorConstants.ordinaryBackColor},
            { key: MButton.FBtnType.Suggest, value: colorConstants.suggestBackColor},
            { key: MButton.FBtnType.Warning, value: colorConstants.warningBackColor}
        ]
        property var hoverColorMap: [
            { key: MButton.FBtnType.Ordinary, value: colorConstants.ordinaryHoverColor},
            { key: MButton.FBtnType.Suggest, value: colorConstants.suggestHoverColor},
            { key: MButton.FBtnType.Warning, value: colorConstants.warningHoverColor}
        ]
        property var clickedColorMap: [
            { key: MButton.FBtnType.Ordinary, value: colorConstants.ordinaryClickedColor},
            { key: MButton.FBtnType.Suggest, value: colorConstants.suggestClickedColor},
            { key: MButton.FBtnType.Warning, value: colorConstants.warningClickedColor}
        ]

        color: {
            if (btnState === MButton.FBtnState.Normal) {
                return backgroundColorMap[btnType].value
            } else {
                return hoverColorMap[btnType].value
            }
        }

        border.width: 2
        border.color: clickedColorMap[btnType].value
        radius: 10

        Behavior on color {
            PropertyAnimation {
                duration: 300
            }
        }

        Behavior on scale {
            PropertyAnimation {
                duration: 300
                easing.type: Easing.OutBack
            }
        }

        ParallelAnimation {
            id: clickRectAni
            PropertyAnimation{
                from: 0.1
                to: 0.5
                easing.type: Easing.OutQuint
                target: clickedRect
                property: "scale"
                duration: 1000
            }

            PropertyAnimation{
                from: 1
                to: 0
                easing.type: Easing.OutQuint
                target: clickedRect
                property: "opacity"
                duration: 1000
                onFinished: {
                    clickedRect.visible = false
                }
            }
        }

        onPressed: {
            clickedRect.visible = true
            clickRectAni.restart()
        }

        Text {
            id: label
            anchors.centerIn: parent
            font.pixelSize: root.fontSize
            font.family: "黑体"
            color: btnRect.clickedColorMap[btnRect.btnType].value
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        Rectangle {
            id: clickedRect
            visible: false
            width: 100
            height: 100
            x: btnRect.mouseClickCenterX - width / 2
            y: btnRect.mouseClickCenterY - height / 2
            radius: 100
            color: btnRect.clickedColorMap[btnType].value
        }

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            onClicked: {
                root.clicked()
            }
            onEntered: {
                btnRect.btnState = MButton.FBtnState.Hover
                btnRect.scale = 1.1
            }
            onExited: {
                btnRect.btnState = MButton.FBtnState.Normal
                btnRect.scale = 1.0
            }
            onPressed: {
                btnRect.btnState = MButton.FBtnState.Clicked
                btnRect.mouseClickCenterX = mouseX
                btnRect.mouseClickCenterY = mouseY
                btnRect.pressed()
            }
            onReleased: {
                btnRect.btnState = MButton.FBtnState.Hover
            }
        }
    }
}
