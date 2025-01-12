import QtQuick
import QtQuick.Controls
import App

Item {

    width: 100
    height: 30

    id: root

    property alias editItem: edit
    property string placeholderText: ""

    Rectangle {
        id: warpper
        anchors.fill: parent
        color: App.normalColorWeight1
        radius: 10
        state: "unInput"

        Behavior on color {
            ColorAnimation{
                duration: 200
            }
        }

        states: [
            State {
                name: "unInput"
                PropertyChanges { target: placeholderItemRect; color: edit.length !== 0 ? App.baseColor: "transparent"; x: edit.length !== 0 ? 15 : 5; y: edit.length !== 0 ? (-((placeholderItem.height + 5) / 2)) : (root.height / 2 - placeholderItemRect.height / 2)}
                PropertyChanges { target: placeholderItem; color: App.windowTextColor}
                PropertyChanges { target: warpper; color: App.normalColorWeight1;}
            },
            State {
                name: "input"
                PropertyChanges { target: placeholderItemRect; color: App.suggestColorWeight3; x: 15; y: -((placeholderItem.height + 5) / 2)}
                PropertyChanges { target: placeholderItem; color: App.windowTextColor}
                PropertyChanges { target: warpper; color: App.suggestColorWeight2;}
            }
        ]

        transitions: [
            Transition {
                from: "unInput"; to: "input"
                PropertyAnimation { target: placeholderItem; properties: "color"; duration: 300 }
                PropertyAnimation { target: placeholderItemRect; properties: "x, y"; duration: 300; easing.type: Easing.OutQuad}
                PropertyAnimation { target: placeholderItemRect; properties: "color"; duration: 300 }
                PropertyAnimation { target: warpper; properties: "color"; duration: 300 }
            },
            Transition {
                from: "input"; to: "unInput"
                PropertyAnimation { target: placeholderItem; properties: "color"; duration: 300 }
                PropertyAnimation { target: placeholderItemRect; properties: "x, y"; duration: 300; easing.type: Easing.OutQuad}
                PropertyAnimation { target: placeholderItemRect; properties: "color"; duration: 300 }
                PropertyAnimation { target: warpper; properties: "color"; duration: 300 }
            }
        ]

        Rectangle {
            id: placeholderItemRect
            height: placeholderItem.height + 5
            width: placeholderItem.width + 5
            color: App.normalColorWeight3
            radius: 5
            Text {
                id: placeholderItem
                anchors.centerIn: parent
                clip: true
                text: placeholderText
                verticalAlignment: Text.AlignVCenter
                color: App.windowTextColor
                font.pixelSize: 10

                Behavior on color {
                    ColorAnimation{
                        duration: 200
                    }
                }
            }
        }

        TextInput  {
            id: edit
            anchors.fill: parent
            anchors.margins: 5
            clip: true
            verticalAlignment: Text.AlignVCenter
            color: App.windowTextColor
            Behavior on color {
                ColorAnimation{
                    duration: 200
                }
            }

            onFocusChanged: {
                if (focus) {
                    parent.state = "input"
                } else {
                    parent.state = "unInput"
                }
            }
        }
    }
}
