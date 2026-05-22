import QtQuick
import FluentUI
import NotificationController 1.0

Item {
    id: root
    z: 9999
    anchors.fill: parent

    Connections {
        target: NotificationController
        function onRequestNotification() {
            var comp = Qt.createComponent("qrc:/qml2/components/NotifItem.qml")
            if (comp.status === Component.Ready) {
                var item = comp.createObject(root, {
                    titleText: NotificationController.title,
                    contentText: NotificationController.content,
                    notifType: NotificationController.type,
                    displayDuration: NotificationController.duration || 3000
                })
                item.show()
            }
        }
    }
}
