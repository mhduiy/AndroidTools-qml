import QtQuick
import QtQuick.Controls
import App

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
            { key: MButton.FBtnType.Ordinary, value: App.normalColorWeight1 },
            { key: MButton.FBtnType.Suggest, value: App.suggestColorWeight1 },
            { key: MButton.FBtnType.Warning, value: App.warningColorWeight1 }
        ]
        property var hoverColorMap: [
            { key: MButton.FBtnType.Ordinary, value: App.normalColorWeight2 },
            { key: MButton.FBtnType.Suggest, value: App.suggestColorWeight2 },
            { key: MButton.FBtnType.Warning, value: App.warningColorWeight2 }
        ]
        property var clickedColorMap: [
            { key: MButton.FBtnType.Ordinary, value: App.normalColorWeight3 },
            { key: MButton.FBtnType.Suggest, value: App.suggestColorWeight3 },
            { key: MButton.FBtnType.Warning, value: App.warningColorWeight3 }
        ]

        color: {
            if (btnState === MButton.FBtnState.Normal) {
                return backgroundColorMap[btnType].value
            } else {
                return hoverColorMap[btnType].value
            }
        }

        Behavior on color {
            ColorAnimation{
                duration: 200
            }
        }
    
        radius: 10

        Behavior on color {
            PropertyAnimation {
                duration: 300
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
            Behavior on color {
                ColorAnimation{
                    duration: 200
                }
            }
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
            }
            onExited: {
                btnRect.btnState = MButton.FBtnState.Normal
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
