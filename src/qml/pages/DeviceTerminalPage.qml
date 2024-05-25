import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import MFloat
import QMLTermWidget 1.0

Item {
    property bool isCurrentPage: false
    id: root
    Layout.minimumWidth: 780
    ColorConstants {
        id: colorConstants
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.rightMargin: 10
        anchors.bottomMargin: 10

        MFrame {
            Layout.fillHeight: true
            Layout.fillWidth: true
            wrapperColor: Qt.rgba(255, 255, 255, 0.65)
            Action{
                onTriggered: terminal.copyClipboard();
                shortcut: "Ctrl+Shift+C"
            }

            Action{
                onTriggered: terminal.pasteClipboard();
                shortcut: "Ctrl+Shift+V"
            }

            Action{
                onTriggered: searchButton.visible = !searchButton.visible
                shortcut: "Ctrl+F"
            }

            Action{
                onTriggered:{
                    console.log('open new terminal window in:'+mainsession.currentDir)
                }
                shortcut: "Ctrl+Shift+T"
            }

            QMLTermWidget {
                id: terminal
                anchors.fill: parent
                anchors.margins: 10
                font.family: 'Source Code Pro'
                font.pointSize: 12
                colorScheme: "BlackOnWhite"
                terminalOpacity: 0
                session: QMLTermSession{
                    id: mainsession
                    initialWorkingDirectory: "$HOME"
                    onMatchFound: {
                        console.log("found at: %1 %2 %3 %4".arg(startColumn).arg(startLine).arg(endColumn).arg(endLine));
                    }
                    onNoMatchFound: {   
                        console.log("not found");
                    }
                }
                onTerminalUsesMouseChanged: console.log(terminalUsesMouse);
                onTerminalSizeChanged: console.log(terminalSize);
                Component.onCompleted: {
                    mainsession.setShellProgram("/usr/bin/zsh")
                    mainsession.startShellProgram();
                    forceActiveFocus()
                }

                QMLTermScrollbar {
                    terminal: terminal
                    width: 20
                    Rectangle {
                        opacity: 0.4
                        color: "#008c8c"
                        anchors.margins: 5
                        radius: width * 0.5
                        anchors.fill: parent
                    }
                }
                MouseArea {
                    anchors.fill: parent
                    propagateComposedEvents: true
                    onClicked: {
                        terminal.forceActiveFocus()
                    }
                }
            }

        }

        MFrame {
            Layout.fillWidth: true
            Layout.preferredHeight: 80
            wrapperColor: Qt.rgba(255, 255, 255, 0.65)
            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 10
                RowLayout {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Text {
                        text: "切换到adb shell，进入设备shell环境"
                    }
                    Item {
                        Layout.fillWidth: true
                    }

                    Timer {
                        id: terminalStartTimer
                        interval: 100
                        repeat: false
                        onTriggered: {
                            mainsession.setShellProgram("/usr/bin/zsh")
                            mainsession.startShellProgram();
                            forceActiveFocus()
                        }
                    }

                    MSwitchButton {
                        onStatusChanged: {
                            if (status) {
                                mainsession.setShellProgram("/usr/bin/zsh")
                                mainsession.startShellProgram();
                                mainsession.sendText("adb shell \n")
                                forceActiveFocus()
                            } else {
                                mainsession.sendText('exit \n')
                                terminalStartTimer.start()
                            }
                        }
                    }
                }
                RowLayout {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    MButton {
                        btnType: MButton.FBtnType.Ordinary
                        text: "从剪切板粘贴"
                        Layout.fillWidth: true
                    }
                    MButton {
                        btnType: MButton.FBtnType.Ordinary
                        text: "清除所有"
                        Layout.fillWidth: true
                    }
                    MButton {
                        btnType: MButton.FBtnType.Ordinary
                        text: "打开系统终端"
                        Layout.fillWidth: true
                    }
                }
            }
        }
    }
}
