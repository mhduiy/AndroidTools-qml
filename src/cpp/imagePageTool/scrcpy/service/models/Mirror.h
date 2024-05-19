#pragma once

#include <QString>

class Mirror {

public:
    Mirror() {};

    QString wifiIp = "";
    QString username = "";
    QString title = "";
    int bitrate = 2;
    int resolution = 1080;
    QString connectionType = "";
};
