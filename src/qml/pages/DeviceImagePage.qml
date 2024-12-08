import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import MFloat
import NotificationController 1.0
import DeviceControl 1.0
import MirrorScene 1.0
import Resource 1.0
import ScrcpyConfig 1.0

ItemPage {
    id: root
    ColorConstants {
        id: colorConstants
    }
    RowLayout {
        anchors.fill: parent
        Rectangle {
            // Layout.fillHeight: true
            Layout.preferredHeight: 560
            Layout.preferredWidth: 270
            Layout.maximumWidth: 270
            Layout.minimumWidth: 270
            radius: 10
            color: "black"

           MirrorScene {
               id: video
               anchors.fill: parent
               anchors.margins: 5
               visible: true

               property int enum_WINDOW_HOME: 0
               property int enum_WINDOW_MIRROR_PORTRATE: 1
               property int enum_WINDOW_MIRROR_LANDSCAPE: 2
               property int enum_WINDOW_MIRROR_FULLSCREEN: 3
               property int enum_WINDOW_MIRROR_MUSIC: 4
               property int enum_WINDOW_MIRROR_SMALL: 5

               Component.onCompleted: {
                    console.warn("||||||||||||")
               }

               onCppGenerateEvents: {
                   switch (request) {
                   case "MIRROR_START":
                    //    mirrorApp.close()
                    console.warn("+++++")
                       Resource.mirror = 1
                       Resource.scene = enum_WINDOW_MIRROR_PORTRATE
                       break
                   case "MIRROR_FINISHED":
                       Resource.mirror = 0
                       break
                   case "FRAME_SIZE_CHANGED":
                       break
                   case "DISPLAY_ORIENTATION_CHANGED":

                       if (Resource.orientation === 0) {
                           // Portrait orientation is vertical
                           Resource.scene = enum_WINDOW_MIRROR_PORTRATE
                       } else if (Resource.orientation === 1) {
                           //Landscape orientation is horizontal
                           console.log("enum_WINDOW_MIRROR_LANDSCAPE")
                           Resource.scene = enum_WINDOW_MIRROR_LANDSCAPE
                       }
                       break
                   case "USB_DEVICE_NAME":
                    //    mirrorApp.setUsbDevice(data)
                       break
                   }
               }
           }
        }
        ColumnLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            MWrapper {
                Layout.fillWidth: true
                Layout.preferredHeight: 120
                title: "实时数据"
                ColumnLayout {
                    RowLayout {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        MLabel {
                            vMargin: 4
                            hMargin: 4
                            text: "码率"
                        }
                        Item {
                            Layout.fillWidth: true
                        }

                        Text {
                            Layout.preferredWidth: 100
                            text: "123Mbps"
                        }
                    }
                    RowLayout {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 30
                        MLabel {
                            vMargin: 4
                            hMargin: 4
                            text: "帧率"
                        }
                        Item {
                            Layout.fillWidth: true
                        }
                        Text {
                            Layout.preferredWidth: 100
                            text: "60fps"
                        }
                    }
                    RowLayout {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 30
                        MLabel {
                            vMargin: 4
                            hMargin: 4
                            text: "坐标"
                        }
                        Item {
                            Layout.fillWidth: true
                        }
                        Text {
                            Layout.preferredWidth: 100
                            text: "1920, 1080"
                        }
                    }
                }
            }
            MWrapper {
                Layout.fillWidth: true
                Layout.preferredHeight: 80
                GridLayout {
                    columns: 4
                    MButton {
                        Layout.fillWidth: true
                        text: "电源"
                        onClicked: {
                            DeviceControl.control(DeviceControl.CTRL_Key, DeviceControl.Power)
                        }
                    }
                    MButton {
                        Layout.fillWidth: true
                        text: "V +"
                        onClicked: {
                            DeviceControl.control(DeviceControl.CTRL_Music, DeviceControl.VolumeAdd)
                        }
                    }
                    MButton {
                        Layout.fillWidth: true
                        text: "V -"
                        onClicked: {
                            DeviceControl.control(DeviceControl.CTRL_Music, DeviceControl.VolumeReduce)
                        }
                    }
                    MButton {
                        Layout.fillWidth: true
                        text: "连接"
                        btnType: MButton.FBtnType.Suggest
                        onClicked:  {
                            Resource.qmlRequest("REQUEST_MIRROR_START", "")
                            NotificationController.send("请求连接", "连接请求已经发送", 0, 3000)
                        }
                    }
                    MButton {
                        Layout.fillWidth: true
                        text: "Menu"
                        onClicked: {
                            DeviceControl.control(DeviceControl.CTRL_Key, DeviceControl.Menu)
                        }
                    }
                    MButton {
                        Layout.fillWidth: true
                        text: "Home"
                        onClicked: {
                            DeviceControl.control(DeviceControl.CTRL_Key, DeviceControl.Home)
                        }
                    }
                    MButton {
                        Layout.fillWidth: true
                        text: "Back"
                        onClicked: {
                            DeviceControl.control(DeviceControl.CTRL_Key, DeviceControl.Back)
                        }
                    }
                    MButton {
                        Layout.fillWidth: true
                        text: "断开"
                        btnType: MButton.FBtnType.Warning
                        onClicked:  {
                            Resource.qmlRequest("REQUEST_MIRROR_FINISH", "")
                            NotificationController.send("请求断开", "断开请求已经发送", 0, 3000)
                        }
                    }
                }
            }

            MWrapper {
                Layout.fillWidth: true
                Layout.fillHeight: true
                ColumnLayout {
                    RowLayout {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 30
                        Text {
                            text: "帧率限制"
                        }
                        Item {
                            Layout.fillWidth: true
                        }

                        MLineEdit {
                            id: fpsEdit
                            Layout.preferredWidth: 140
                            placeholderText: "帧率(fps)"
                            editItem.text: ScrcpyConfig.maxFps
                        }

                        MButton {
                            Layout.preferredWidth: 80
                            text: "设置"
                            btnType: MButton.FBtnType.Suggest
                            onClicked: {
                                ScrcpyConfig.maxFps = parseInt(fpsEdit.editItem.text)
                                Resource.qmlRequest("REQUEST_MIRROR_FINISH", "")
                                Resource.qmlRequest("REQUEST_MIRROR_START", "")
                            }
                        }
                    }
                    RowLayout {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 30
                        Text {
                            text: "码率限制"
                        }
                        Item {
                            Layout.fillWidth: true
                        }

                        MLineEdit {
                            id: rateEdit
                            Layout.preferredWidth: 140
                            placeholderText: "码率(Kbps)"
                            editItem.text: ScrcpyConfig.kBitRate
                        }

                        MButton {
                            Layout.preferredWidth: 80
                            text: "设置"
                            btnType: MButton.FBtnType.Suggest
                            onClicked: {
                                ScrcpyConfig.kBitRate = parseInt(rateEdit.editItem.text)
                                Resource.qmlRequest("REQUEST_MIRROR_FINISH", "")
                                Resource.qmlRequest("REQUEST_MIRROR_START", "")
                            }
                        }
                    }
                    RowLayout {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 30
                        Text {
                            text: "屏幕截图"
                        }
                        Item {
                            Layout.fillWidth: true
                        }
                        MLineEdit {
                            id: shotScreenEdit
                            editItem.text: "/home/mhduiy/"
                            Layout.preferredWidth: 140
                            placeholderText: "保存路径"
                        }

                        MButton {
                            Layout.preferredWidth: 80
                            text: "点击截图"
                            btnType: MButton.FBtnType.Suggest
                            onClicked: {
                                ImageDetailTools.shotScreen(shotScreenEdit.editItem.text)
                            }
                        }
                    }
                    RowLayout {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 30
                        Text {
                            text: "屏幕录制"
                        }
                        Item {
                            Layout.fillWidth: true
                        }
                        MLineEdit {
                            id: recordPathEdit
                            editItem.text: ScrcpyConfig.recordOutPath
                            Layout.preferredWidth: 140
                            placeholderText: "保存路径"
                        }

                        MButton {
                            text: "开始录制"
                            Layout.preferredWidth: 80
                            btnType: MButton.FBtnType.Suggest
                            onClicked: {
                                ScrcpyConfig.recordOutPath = recordPathEdit.editItem.text
                                Resource.qmlRequest("REQUEST_MIRROR_FINISH", "")
                                Resource.qmlRequest("REQUEST_MIRROR_START", "")
                                NotificationController.send("开始录制", "断开即结束录制", 0, 3000)
                            }
                        }
                    }
                    RowLayout {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 30
                        Text {
                            text: "按键宏录制"
                        }
                        Item {
                            Layout.fillWidth: true
                        }

                        MLineEdit {
                            Layout.preferredWidth: 140
                            placeholderText: "宏名称"
                        }

                        MButton {
                            Layout.preferredWidth: 80
                            text: "开始录制"
                            btnType: MButton.FBtnType.Suggest
                        }
                    }
                    RowLayout {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 30
                        Text {
                            text: "执行宏"
                        }
                        Item {
                            Layout.fillWidth: true
                        }

                        MLineEdit {
                            Layout.preferredWidth: 140
                            placeholderText: "宏名称"
                        }

                        MButton {
                            Layout.preferredWidth: 80
                            text: "开始执行"
                            btnType: MButton.FBtnType.Suggest
                        }
                    }
                    RowLayout {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 30
                        Text {
                            text: "宏重复执行"
                        }
                        Item {
                            Layout.fillWidth: true
                        }

                        MSwitchButton {
                            Layout.preferredWidth: 50
                        }
                    }
                    RowLayout {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 30
                        Text {
                            text: "宏执行间隔"
                        }
                        Item {
                            Layout.fillWidth: true
                        }

                        MLineEdit {
                            placeholderText: "间隔(ms)"
                            Layout.preferredWidth: 140
                        }
                    }
                }
            }
        }
    }
}
