import QtQuick
import QtQuick.Controls
import QtQuick.Templates as T
import App

T.Slider {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitHandleWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitHandleHeight + topPadding + bottomPadding)


    background: Rectangle {
        // implicitWidth: 200
        implicitHeight: 10
        color: App.adjustColorOpacity(App.normalColorWeight3, 0.15)
        radius: 4
    }

    handle: Rectangle {
        color: App.adjustColorOpacity(App.suggestColorWeight3, 1.0)
        width: 15
        height: 20
        radius: 4
        x: control.leftPadding + (control.horizontal ? control.visualPosition * (control.availableWidth - width) : (control.availableWidth - width) / 2)
        y: control.topPadding + (control.horizontal ? (control.availableHeight - height) / 2 : control.visualPosition * (control.availableHeight - height))
    }
}