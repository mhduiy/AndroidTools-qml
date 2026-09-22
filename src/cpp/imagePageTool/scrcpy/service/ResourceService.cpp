#include "ResourceService.h"

ResourceService::ResourceService(QObject *parent) : QObject{parent} {
    mirror = new Mirror();
}

bool ResourceService::getMirror() const {
    return m_mirror;
}

QSize ResourceService::frameSize() const {
    return m_frameSize;
}

int ResourceService::orientation() const {
    return m_orientation;
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

void ResourceService::setMirror(bool value) {
    m_mirror = value;
    emit mirrorChanged(m_mirror);
}

void ResourceService::setFrameSize(QSize size) {

    qDebug() << "ResourceService::setFrameSize";

    m_frameSize = size;

    if (size.height() > size.width()) {
        setOrientation(DisplayOrientation::PORTRAIT);
    } else {
        setOrientation(DisplayOrientation::LANDSCAPE);
    }

    emit frameSizeChanged(m_frameSize);
    //emit cppGenerateEvents("FRAME_SIZE_CHANGED", "");
}

void ResourceService::setOrientation(int orientation) {
    m_orientation = orientation;
    emit orientationChanged(m_orientation);
    emit cppGenerateEvents("DISPLAY_ORIENTATION_CHANGED", "");
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
