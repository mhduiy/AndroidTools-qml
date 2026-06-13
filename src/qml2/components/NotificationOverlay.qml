import QtQuick
import NotificationController 1.0

Item {
    id: root
    z: 9999
    anchors.fill: parent

    property var notifications: []
    readonly property int margin: 18
    readonly property int gap: 10
    readonly property int maxVisible: 5

    function relayout() {
        var nextY = margin
        for (var i = 0; i < notifications.length; ++i) {
            var item = notifications[i]
            if (!item)
                continue
            item.stackIndex = i
            item.targetY = nextY
            item.x = item.targetX
            nextY += item.height + gap
        }
    }

    function removeNotification(item) {
        var next = []
        for (var i = 0; i < notifications.length; ++i) {
            if (notifications[i] !== item)
                next.push(notifications[i])
        }
        notifications = next
        relayout()
        if (item)
            item.destroy()
    }

    function pushNotification() {
        var comp = Qt.createComponent("qrc:/qml2/components/NotifItem.qml")
        if (comp.status !== Component.Ready)
            return

        var item = comp.createObject(root, {
            titleText: NotificationController.title,
            contentText: NotificationController.content,
            notifType: NotificationController.type,
            displayDuration: NotificationController.duration || 3000,
            targetY: margin
        })
        if (!item)
            return

        item.closed.connect(function(closedItem) {
            root.removeNotification(closedItem)
        })

        var next = notifications.slice()
        next.unshift(item)
        notifications = next
        relayout()
        item.show()

        while (notifications.length > maxVisible) {
            notifications[notifications.length - 1].dismiss()
            break
        }
    }

    Connections {
        target: NotificationController
        function onRequestNotification() {
            root.pushNotification()
        }
    }
}
