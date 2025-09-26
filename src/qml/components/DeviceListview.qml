import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import MFloat
import App
import DeviceHelper 1.0
import ConnectManager 1.0

ListView {
    id: listView
    clip: true
    model: DeviceHelper.adbDeviceList
    delegate: Rectangle {
        width: ListView.view.width
        height: 75
        radius: 10
        color: ListView.isCurrentItem ? Qt.rgba(0, 0, 0, 0.2) : Qt.rgba(0, 0, 0, 0.05)

        Behavior on color {
            PropertyAnimation {
                duration: 300
            }
        }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 6

            spacing: 2
            RowLayout {
                Layout.fillWidth: true
                Label {
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
                    text: (modelData && modelData.isConnected) ? (modelData.isWireless ? "无线" : "有线") : "历史"
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
                        text: {
                            let cutValue = modelData ? modelData.batteryLevel : 0
                            let cutPower = modelData ? Math.floor((modelData.batteryCurrent * modelData.batteryVoltage) / 1000000) : 0
                            return (modelData && modelData.isConnected) ?
                                (modelData.isCharging ? cutValue + "%+" : cutValue + "%") + "(" + cutPower + "w)" : "未连接"
                        }
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
