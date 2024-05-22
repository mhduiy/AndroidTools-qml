#include <QtQml>

#include "../core/include/QtScrcpyCore.h"
#include "ResourceService.h"

ResourceService::ResourceService(QObject *parent) : QObject{parent} {
    m_portraitSize.setWidth(375);
    m_portraitSize.setHeight(830);
    mirror = new Mirror();
}

void ResourceService::declareQml() {
    qmlRegisterType<ResourceService>("App", 1, 0, "ResourceService");
}

bool ResourceService::getMirror() const {
    return m_mirror;
}

QSize ResourceService::frameSize() const {
    return m_frameSize;
}

QSize ResourceService::portraitSize() const {
    return m_portraitSize;
}

QSize ResourceService::landscapeSize() const {
    return m_landscapeSize;
}

int ResourceService::orientation() const {
    return m_orientation;
}
int ResourceService::scene() const {
    return m_scene;
}

void ResourceService::setSerial(const QString &serial) {
    m_serial = serial;
}

QString ResourceService::serial() {
    return m_serial;
}

void ResourceService::qmlRequest(QString request, QString data) {
    emit qmlGenerateEvents(request, data);
}

void ResourceService::qmlToolbarClick(QString request) {

    qDebug() << "Type:" << request;

    auto device = qsc::IDeviceManage::getInstance().getDevice(m_serial);
    if (!device) {
        return;
    }

    if (request == "home") {
        device->postGoBack();
    } else if (request == "volume-up") {
        device->postVolumeUp();
    } else if (request == "volume-down") {
        device->postVolumeDown();
    } else if (request == "arrow-back") {
        device->postGoBack();
    } else if (request == "screen-on") {
        device->setScreenPowerMode(true);
    } else if (request == "screen-off") {
        device->setScreenPowerMode(false);
    } else if (request == "power") {
        device->postPower();
    } else if (request == "expand-menu") {
        device->expandNotificationPanel();
    } else if (request == "app-switch") {
        device->postAppSwitch();
    } else if (request == "touch-show") {
        m_showTouch = !m_showTouch;
        device->showTouch(m_showTouch);
    } else if (request == "app-switch") {
        device->postAppSwitch();
    }
}

void ResourceService::setMirror(bool value) {
    m_mirror = value;
    emit mirrorChanged(m_mirror);
}

void ResourceService::setFrameSize(QSize size) {

    qDebug() << "ResourceService::setFrameSize";

    m_frameSize = size;

    if (size.height() > size.width()) {
        m_portraitSize.setWidth(488 / 1.5 );
        m_portraitSize.setHeight(1080 / 1.5);
        setOrientation(DisplayOrientation::PORTRAIT);
    } else {
        float scale = 1.35;
        m_landscapeSize.setWidth(1080 * scale);
        m_landscapeSize.setHeight(488 * scale);
        setOrientation(DisplayOrientation::LANDSCAPE);
    }

    //emit frameSizeChanged(m_frameSize);
    //emit cppGenerateEvents("FRAME_SIZE_CHANGED", "");
}

void ResourceService::setOrientation(int orientation) {
    m_orientation = orientation;
    emit orientationChanged(m_orientation);
    emit cppGenerateEvents("DISPLAY_ORIENTATION_CHANGED", "");
}

void ResourceService::setScene(int scene) {
    m_scene = scene;
    emit sceneChanged(scene);
}

void ResourceService::setPortraitSize(QSize size) {
    m_portraitSize = size;
    emit portraitSizeChanged(m_portraitSize);
}

void ResourceService::setLandscapeSize(QSize size) {
    m_landscapeSize = size;
    emit landscapeSizeChanged(m_landscapeSize);
}

void ResourceService::setUsbMirrorParametre() const {
    mirror->username = "USB Mirror";
    mirror->title = "screen is mirroring by USB";
    mirror->bitrate = 5;
    mirror->resolution = 1080;
    mirror->connectionType = "USB";
}

void ResourceService::clearMirrorCash() {
    mirror->wifiIp = "";
    mirror->username = "";
    mirror->title = "";
    mirror->bitrate = 5;
    mirror->resolution = 1080;
    mirror->connectionType = "";
}


