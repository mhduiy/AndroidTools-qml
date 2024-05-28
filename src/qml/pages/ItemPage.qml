import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    property bool isCurrentPage: false
    id: page
    Layout.minimumWidth: 780
    state: "prepareShow"
    states: [
        State {
            name: "pageShow"
            PropertyChanges {
                target: page
                opacity: 1
                scale: 1
            }
        },
        State {
            name: "prepareShow"
            PropertyChanges {
                target: page
                opacity: 0
                scale: 0.85
            }
        }
    ]
    transitions: [
        Transition {
            from: "pageShow"
            to: "prepareShow"
            PropertyAnimation { target: page; properties: "opacity, scale"; duration: 800; easing.type: Easing.OutExpo}
        },
        Transition {
            from: "prepareShow"
            to: "pageShow"
            PropertyAnimation { target: page; properties: "opacity, scale";duration: 800; easing.type: Easing.OutExpo}
        }
    ]
}
