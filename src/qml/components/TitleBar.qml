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
            property int leftMargin: 0
            text: "AndroidTools"
            Layout.leftMargin: leftMargin
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 14

            Behavior on scale {
                PropertyAnimation {
                    duration: 1500
                    easing.type: Easing.OutBack
                }
            }
            Behavior on leftMargin {
                PropertyAnimation {
                    duration: 800
                    easing.type: Easing.OutQuart
                }
            }
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onEntered: {
                    parent.scale = 1.6
                    parent.leftMargin = 30
                    eggTip.showTipTip()
                }
                onExited: {
                    parent.scale = 1.0
                    parent.leftMargin = 0
                }
                onClicked: {
                    eggTip.showTip()
                }
            }
        }

        Text {
            id: eggTip
            font.bold: true
            font.pixelSize: 14
            property int gongde: 0
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            Layout.leftMargin: 50
            color: "#E67E22"
            opacity: 0

            Behavior on opacity {
                PropertyAnimation {
                    duration: 1000
                    easing.type: Easing.InQuart
                }
            }

            function showTip() {
                if (!eggTipTimer.running) {
                    gongde = 0
                }

                gongde++
                opacity = 1.0
                text = "功德 +" + gongde
                eggTipTimer.restart()
            }
            function hideTip() {
                if (gongde < 10 && gongde !== 0) {
                    text = "功德不足  (¬_¬)"
                } else if (gongde >= 10){
                    text = "功德满满  Ψ(￣∀￣)Ψ"
                }

                gongde = 0
                opacity = 0
            }
            function showTipTip() {
                if (gongde !== 0) {
                    return
                }

                opacity = 1.0
                text = "<----点它点它  (￣m￣）"
                eggTipTimer.start()
            }
        }

        Timer {
            id: eggTipTimer
            interval: 1500
            repeat: false
            onTriggered: {
                eggTip.hideTip()
            }
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
