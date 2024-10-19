import QtQuick
import QtQuick.Controls

Item {

    width: 100
    height: 30

    id: root

    property alias editItem: edit
    property string placeholderText: ""

    ColorConstants {
        id: colorConstants
    }

    Rectangle {
        id: warpper
        anchors.fill: parent
        color: colorConstants.ordinaryBackColor
        radius: 10
        border.width: 2
        border.color: colorConstants.ordinaryClickedColor

        state: "unInput"

        states: [
            State {
                name: "unInput"
                PropertyChanges { target: placeholderItemRect; color: edit.length !== 0 ? colorConstants.ordinaryClickedColor: "transparent"; x: edit.length !== 0 ? 15 : 5; y: edit.length !== 0 ? (-((placeholderItem.height + 5) / 2)) : (root.height / 2 - placeholderItemRect.height / 2)}
                PropertyChanges { target: placeholderItem; color: edit.length !== 0 ? "white" : "gray"}
                PropertyChanges { target: warpper; color: colorConstants.ordinaryBackColor; border.color: colorConstants.ordinaryClickedColor}
            },
            State {
                name: "input"
                PropertyChanges { target: placeholderItemRect; color: colorConstants.suggestClickedColor; x: 15; y: -((placeholderItem.height + 5) / 2)}
                PropertyChanges { target: placeholderItem; color: "white"}
                PropertyChanges { target: warpper; color: colorConstants.suggestBackColor; border.color: colorConstants.suggestClickedColor}
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
            color: colorConstants.ordinaryClickedColor
            radius: 5
            Text {
                id: placeholderItem
                anchors.centerIn: parent
                clip: true
                text: placeholderText
                verticalAlignment: Text.AlignVCenter
                color: "white"
                font.pixelSize: 10
            }
        }

        TextInput  {
            id: edit
            anchors.fill: parent
            anchors.margins: 5
            clip: true
            verticalAlignment: Text.AlignVCenter

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
