#pragma once

#include <QString>

namespace qsc {

    struct DeviceParams {
        // necessary
        QString serial = "";              // devise serial number
        QString serverLocalPath = "";     // Local Android server path

        // optional
        QString serverRemotePath =        "/data/local/tmp/scrcpy-server.jar";    // The server path to be pushed to the remote device
        quint16 localPort = 27183;        // Local listening port in reverse
        quint16 maxSize = 720;            // video resolution
        quint32 bitRate = 2000000;        // video bitrate
        quint32 maxFps = 0;               // Video Maximum Frame Rate
        bool useReverse = true;           // true: use adb reverse first, and automatically use adb forward after failure; false: use adb forward directly
        int lockVideoOrientation = -1;    // Whether to lock the video orientation
        bool stayAwake = false;           // Whether to stay awake
        QString serverVersion = "1.24";   // server version

        QString logLevel = "debug";       // log level verbose/debug/info/warn/error
        // Encoding option "" means default
        // For example CodecOptions="profile=1,level=2"
        // More encoding options reference https://d.android.com/reference/android/media/MediaFormat
        QString codecOptions = "";

        // Specify the encoder name (must be H.264 encoder), "" means the default
        // For example CodecName="OMX.qcom.video.encoder.avc"
        QString codecName = "";

        QString recordPath = "";          // Video save path
        QString recordFileFormat = "mp4"; // video save format mp4/mkv
        bool recordFile = false;          // record to file

        QString pushFilePath = "/sdcard/"; // The file save path pushed to the Android device (must end with /)

        bool closeScreen = false;         // Automatically turn off the screen at startup
        bool display = true;              // Whether to show the screen (or just record in the background)
        bool renderExpiredFrames = false; // Whether to render delayed video frames
        QString gameScript = "";          // Game Mapping Script
    };

}
