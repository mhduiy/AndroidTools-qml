import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import MFloat
import App
import DeviceHelper 1.0
import ConnectManager 1.0

Item {
    height: 400
    width: 20
    id: root
    
    ListView {
        id: listView
        anchors.fill: parent
        clip: true
        model: DeviceHelper.adbDeviceList
        highlightMoveDuration: 300
        highlight: Rectangle {
            color: App.themeType === App.Light ? 
                Qt.rgba(135 / 255, 206 / 255, 250 / 255, 0.6) : Qt.rgba(0 / 255, 49 / 255, 49 / 255, 0.6)
            radius: 10
        }
        delegate: lvDelegate
        spacing: 10
    }

    Component {
        id: lvDelegate
        Rectangle {
            width: ListView.view.width
            height: 75
            radius: 10
            color: Qt.rgba(255,255,255, ListView.isCurrentItem ? 0 : 0.4)

            Behavior on color {
                PropertyAnimation {
                    duration: 300
                }
            }

            ColumnLayout {
                anchors.fill: parent
                anchors.topMargin: 3
                anchors.bottomMargin: 3
                anchors.leftMargin: 5
                anchors.rightMargin: 5
                spacing: 2
                RowLayout {
                    Layout.fillWidth: true
                    Text {
                        id: deviceName
                        text: {
                            if (modelData) {
                                // 优先显示制造商和型号
                                if (modelData.manufacturer && modelData.model) {
                                    return modelData.manufacturer + " " + modelData.model
                                } else if (modelData.manufacturer) {
                                    return modelData.manufacturer + " 设备"
                                } else if (modelData.deviceName && modelData.deviceName !== "") {
                                    return modelData.deviceName
                                } else if (modelData.code) {
                                    return modelData.code
                                }
                            }
                            return "未知设备"
                        }
                        font.family: "黑体"
                        font.bold: true
                        font.pixelSize: 14
                        color: "#008c8c"
                    }

                    Item {
                        Layout.fillWidth: true // 占据剩余空间
                    }

                    MLabel {
                        text: (modelData && modelData.isConnected) ? (modelData.isWireless ?  "无线" : "有线") : "历史"
                        textColor: "#ffffff"
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    GridLayout {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        columnSpacing: 1
                        rowSpacing: 1
                        columns: 2

                        MLabel {
                            text: "IP"
                            vMargin: 1
                            hMargin: 4
                            textColor: "#ffffff"
                            rectColor: "#000000"
                        }
                        Label {
                            text: (modelData && modelData.isConnected) ? (modelData.isWireless && modelData.ip ? modelData.ip : "有线连接") : "未连接"
                            font.pixelSize: 12
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }

                        MLabel {
                            text: "BT"
                            vMargin: 1
                            hMargin: 3
                            textColor: "#ffffff"
                            rectColor: "#000000"
                        }
                        Label {
                            text: (modelData && modelData.isConnected) ? (modelData.isCharging ? modelData.batteryLevel + "% +" : modelData.batteryLevel + "%") : "未连接"
                            font.pixelSize: 12
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                    }

                    Item {
                        Layout.fillWidth: true // 占据剩余空间
                    }

                    MButton {
                        Layout.preferredWidth: 50
                        Layout.alignment: Qt.AlignBottom
                        btnType: (modelData && modelData.isConnected) ? MButton.FBtnType.Warning : MButton.FBtnType.Suggest
                        text: (modelData && modelData.isConnected) ? "断开" : "立即连接"
                        z: 1000
                        onClicked: {
                            if (modelData && modelData.code) {
                                DeviceHelper.requestDisConnectDevice(modelData.code)
                            }
                        }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if (modelData && modelData.code) {
                                    DeviceHelper.requestDisConnectDevice(modelData.code)
                                    console.log("断开连接:", modelData.code)
                                }
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
                    ConnectManager.requestSetCutADBDevice(modelData.code)
                }
            }
        }
    }
}
