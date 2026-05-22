import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects
import FluentUI
import WallpaperHelper 1.0
import "./components"

FluWindow {
    id: mainWindow
    width: 960
    height: 640
    title: "AndroidTools"
    launchMode: FluWindowType.Standard
    fitsAppBarWindows: true
    minimumWidth: 600
    minimumHeight: 400

    appBar: FluAppBar {
        height: 30
        showDark: true
    }

    // ---- 壁纸 + 模糊背景 ----
    Item {
        anchors.fill: parent
        z: -1

        Image {
            id: wallpaperImage
            anchors.fill: parent
            source: WallpaperHelper.wallpaper
            fillMode: Image.PreserveAspectCrop
            asynchronous: true
            visible: false
        }

        FastBlur {
            anchors.fill: parent
            source: wallpaperImage
            radius: WallpaperHelper.blurRadius || 50
            transparentBorder: true
        }

        Rectangle {
            anchors.fill: parent
            color: FluTheme.dark ? Qt.rgba(0.12, 0.12, 0.14, 0.85) : Qt.rgba(0.98, 0.98, 0.98, 0.75)
            Behavior on color { ColorAnimation { duration: 300 } }
        }
    }

    // ---- 导航视图 ----
    FluNavigationView {
        id: navView
        width: parent.width
        height: parent.height
        pageMode: FluNavigationViewType.NoStack
        displayMode: FluNavigationViewType.Compact
        cellHeight: 42

        items: FluObject {
            FluPaneItem {
                title: "Device Info"
                icon: FluentIcons.Phone
                url: "qrc:/qml2/pages/DeviceInfoPage.qml"
                onTap: { navView.push(url) }
            }
            FluPaneItem {
                title: "Device Control"
                icon: FluentIcons.Game
                url: "qrc:/qml2/pages/DeviceControlPage.qml"
                onTap: { navView.push(url) }
            }
            FluPaneItem {
                title: "App Manager"
                icon: FluentIcons.AllApps
                url: "qrc:/qml2/pages/DeviceAppPage.qml"
                onTap: { navView.push(url) }
            }
            FluPaneItem {
                title: "Flash Tools"
                icon: FluentIcons.DeveloperTools
                url: "qrc:/qml2/pages/DeviceFlashPage.qml"
                onTap: { navView.push(url) }
            }
            FluPaneItem {
                title: "Screen Mirror"
                icon: FluentIcons.TVMonitor
                url: "qrc:/qml2/pages/DeviceImagePage.qml"
                onTap: { navView.push(url) }
            }
            FluPaneItem {
                title: "ADB Log"
                icon: FluentIcons.CommandPrompt
                url: "qrc:/qml2/pages/LogPage.qml"
                onTap: { navView.push(url) }
            }
            FluPaneItem {
                title: "Settings"
                icon: FluentIcons.Settings
                url: "qrc:/qml2/pages/SettingPage.qml"
                onTap: { navView.push(url) }
            }
        }

        Component.onCompleted: {
            setCurrentIndex(0)
        }
    }

    NotificationOverlay {
        anchors.fill: parent
    }
}
