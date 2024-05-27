import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import MFloat
import DeviceListviewModel 1.0

Item {
    height: 400
    width: 20
    id: root
    ListView {
        id: listView
        anchors.fill: parent
        clip: true
        model: DeviceListviewModel
        delegate: lvDelegate
        spacing: 10

        onCurrentIndexChanged: {
            DeviceListviewModel.setCurrentIndex(currentIndex)
        }
    }

    Component {
        id: lvDelegate
        Rectangle {
            width: ListView.view.width
            height: 70
            radius: 10
            border.width: 1
            border.color: "gray"
            color: ListView.isCurrentItem ? Qt.rgba(135 / 255, 206 / 255, 250 / 255, 0.65) : Qt.rgba(255,255,255, 0.65)

            Behavior on color {
                PropertyAnimation {
                    duration: 100
                }
            }

            ColumnLayout {
                anchors.fill: parent
                spacing: 2
                RowLayout {
                    Layout.fillWidth: true
                    Layout.leftMargin: 5
                    Layout.rightMargin: 5
                    Layout.topMargin: 2
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
                        text: model.isConnected ? model.isWireless ?  "无线" : "有线" : "历史"
                        textColor: "#ffffff"
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    Layout.leftMargin: 5
                    Layout.bottomMargin: 5
                    Layout.rightMargin: 5
                    Layout.fillHeight: true
                    GridLayout {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        columns: 2

                        MLabel {
                            text: "IP"
                            vMargin: 1
                            hMargin: 4
                            textColor: "#ffffff"
                            rectColor: "#000000"
                        }
                        Label {
                            text: model.isConnected ? model.isWireless ? model.ip : "有线连接" : "未连接"
                            font.pixelSize: 12
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
                            text: model.isConnected ? (model.isCharging ? model.battery + "% +" : model.battery + "%") : "未连接"
                            font.pixelSize: 12
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                    }

                    Item {
                        Layout.fillWidth: true // 占据剩余空间
                    }

                    MButton {
                        btnType: model.isConnected ? MButton.FBtnType.Warning : MButton.FBtnType.Suggest
                        text: model.isConnected ? "断开连接" : "立即连接"
                        z: 1000
                        onClicked: {
                            DeviceListviewModel.requestDisConnect(model.deviceCode)
                        }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                DeviceListviewModel.requestDisConnect(model.deviceCode)
                                console.log("断开连接")
                            }
                        }
                    }
                }
            }
            MouseArea {
                anchors.fill: parent
                propagateComposedEvents: true
                onClicked: {
                    listView.currentIndex = index
                    mouse.accepted = false
                }
            }
        }
    }
}
