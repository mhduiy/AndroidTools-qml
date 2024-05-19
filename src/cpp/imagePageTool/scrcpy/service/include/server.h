#pragma once

namespace Server {

    enum CLIENT_REQUEST {
        CLIENT_REQUEST_ADB_CONNECT,       // android client request to connect to adb by WiFi IP #adb connect 192.168.1.1
        CLIENT_REQUEST_UPDATE,            // android client is now connected to server and request get current data for sync
        CLIENT_REQUEST_MIRROR,            // android client request for start screen mirror
    };

    enum SERVER_RESPONSE {
        MIRROR_SUCCESS_START,     // Start successfully
        MIRROR_ERROR_START,       // Startup failed
        MIRROR_FINISHED,          // mirror is finished
        UPDATE_MIRROR_DATA,       // update user interface
    };
}
