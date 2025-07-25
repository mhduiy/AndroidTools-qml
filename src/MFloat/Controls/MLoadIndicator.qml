import QtQuick
import QtQuick.Controls
import App

Item {
    width: 30
    height: 30
    property alias running: rotationAnimation.running
    function start() {
        rotationAnimation.start()
        canvas.opacity = 1.0
    }
    function stop() {
        rotationAnimation.stop()
        canvas.opacity = 0.0
    }
    Canvas {
        id: canvas
        anchors.fill: parent
        opacity: 0.0
        onPaint: {
            var ctx = getContext("2d");
            ctx.strokeStyle = App.normalColorWeight3;
            ctx.lineWidth = 1;
            ctx.imageSmoothingEnabled = true;
            antialiasing: true

            var centerX = width / 2;
            var centerY = height / 2;
            var radius = width / 2 - 2;

            ctx.beginPath();
            ctx.arc(centerX, centerY, radius, 0, 2 * Math.PI);
            ctx.stroke();

            var smallRadius = 4;
            ctx.fillStyle = App.normalColorWeight3;
            ctx.beginPath();
            ctx.arc(centerX, 10, smallRadius, 0, 2 * Math.PI);
            ctx.fill();
        }
        Behavior on opacity {
            PropertyAnimation {
                duration: 300
            }
        }
    }

    Connections {
        target: App
        function onThemeTypeChanged() {
            canvas.requestPaint()
        }
    }

    RotationAnimation {
        id: rotationAnimation
        target: canvas
        property: "rotation"
        from: 0
        to: 360
        duration: 1000
        loops: Animation.Infinite
    }
}
