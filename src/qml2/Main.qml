import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects
import FluentUI
import WallpaperHelper 1.0
import "./components"

FluWindow {
    id: mainWindow
    width: 1180
    height: 760
    title: "AndroidTools"
    launchMode: FluWindowType.Standard
    fitsAppBarWindows: true
    fixSize: true
    minimumWidth: 1180
    minimumHeight: 760
    maximumWidth: 1180
    maximumHeight: 760

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
        anchors.fill: parent
        pageMode: FluNavigationViewType.NoStack
        displayMode: FluNavigationViewType.Compact
        navCompactWidth: 54
        cellHeight: 42

        items: FluObject {
            FluPaneItem {
                title: "控制台"
                icon: FluentIcons.Home
                url: "qrc:/qml2/pages/HomePage.qml"
                onTap: { navView.push(url) }
            }
        }

        footerItems: FluObject {
            FluPaneItem {
                title: "设置"
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
