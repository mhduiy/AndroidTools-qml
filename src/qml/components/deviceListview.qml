import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import MFloat


Item {
    height: 400
    width: 20
    id: root
    ListView {
        id: listView
        anchors.fill: parent
        clip: true
        model: myModel
        delegate: lvDelegate
        spacing: 10
    }

    Component {
        id: lvDelegate
        Rectangle {
            width: ListView.view.width
            height: 70
            radius: 10
            color: ListView.isCurrentItem ? "#FFBCAD" : "#e4e4e4"
            Column {
                anchors.fill: parent
                padding: 5
                spacing: 2
                RowLayout {
                    width: parent.width - 10
                    Text {
                        id: deviceName
                        text: model.deviceName
                        font.family: "黑体"
                        font.bold: true
                        font.pixelSize: 14
                        color: "#008c8c"
                    }

                    Item {
                        Layout.fillWidth: true // 占据剩余空间
                    }

                    MLabel {
                        text: model.isconnected ? model.isWireless ?  "无线" : "有线" : "历史"
                        textColor: "#ffffff"
                    }
                }

                RowLayout {
                    width: parent.width - 10
                    GridLayout {
                        columns: 2

                        MLabel {
                            text: "IP"
                            vMargin: 1
                            hMargin: 4
                            textColor: "#ffffff"
                            rectColor: "#000000"
                        }
                        Label {
                            text: model.isconnected ? model.isWireless ? model.ip : "有线连接" : "未连接"
                            font.pixelSize: 10
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }

                        MLabel {
                            text: "BT"
                            vMargin: 1
                            hMargin: 3
                            textColor: "#ffffff"
                            rectColor: "#000000"                        }
                        Label {
                            text: model.isconnected ? (model.ischarging ? model.battery + "% +" : model.battery + "%") : "未连接"
                            font.pixelSize: 10
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                    }

                    Item {
                        Layout.fillWidth: true // 占据剩余空间
                    }

                    MButton {
                        btnType: model.isconnected ? MButton.FBtnType.Warning : MButton.FBtnType.Suggest
                        text: model.isconnected ? "断开连接" : "立即连接"
                    }
                }
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    listView.currentIndex = index
                }
            }
        }
    }

    ListModel {
        id: myModel
        ListElement {
            deviceName: "XIAOMI Mi8 Lite"
            isconnected: true
            isWireless: true
            ip: "192.168.0.1"
            battery: "50"
            ischarging: false

        }
        ListElement {
            deviceName: "HUAWEI nova 11pro"
            isconnected: true
            isWireless: false
            ip: ""
            battery: "50"
            ischarging: true
        }
        ListElement {
            deviceName: "OPPO R11"
            isconnected: false
            isWireless: false
            ip: ""
            battery: ""
            ischarging: false
        }
    }
}
