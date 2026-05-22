# AndroidTools

[![License](https://img.shields.io/github/license/mhduiy/AndroidTools-qml)](LICENSE)
[![Build](https://github.com/mhduiy/AndroidTools-qml/actions/workflows/compile-project.yml/badge.svg)](https://github.com/mhduiy/AndroidTools-qml/actions/workflows/compile-project.yml)
[![Deb Package](https://github.com/mhduiy/AndroidTools-qml/actions/workflows/build-deb.yml/badge.svg)](https://github.com/mhduiy/AndroidTools-qml/actions/workflows/build-deb.yml)
[![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20macOS%20%7C%20Linux-blue)]()

A cross-platform Android toolbox with modern GUI, built on Qt6 + QML. Think of it as a graphical interface for ADB — manage devices, install apps, control settings, flash firmware, and mirror screens, all without touching the command line.

> Previously [AndroidTools](https://github.com/mhduiy/AndroidTools), rewritten in QML for a fluid, animated UI.

---

## Features

### Device Info
- Hardware details: manufacturer, model, brand, serial number, SDK version
- Real-time battery monitor with charging type, voltage, temperature
- CPU architecture, core count, max frequency
- Memory and storage overview
- Foreground activity tracking

### Device Control
- Music playback control (play/pause, next, previous, volume)
- Hardware key simulation (home, back, power, brightness, camera, etc.)
- Broadcast injection — simulate system events like network change, low battery, boot complete
- Battery spoofing — fake charge level and charging state for testing
- Resolution and DPI modification on the fly
- File push/pull between PC and device
- Keyboard passthrough — type on PC, input goes to device
- Manual activity launcher

### App Management
- List installed apps (user, system, or all) with version info
- Install APK with advanced options (downgrade, overwrite, SD card install)
- Uninstall, force-stop, clear data
- Freeze/unfreeze (disable/enable) apps
- Extract APK to PC
- Launch apps remotely

### Screen Mirroring
- Low-latency screen projection via scrcpy
- Configurable bitrate, resolution, and frame rate
- Screenshot capture
- Touch and click interaction on the mirrored screen

### Flash Tools
- Fastboot device detection and management
- Temporary boot (boot an image without flashing)
- Flash or erase specific partitions
- Flash script execution in terminal
- Xiaomi fastboot flash support
- Firmware package extraction

### Settings
- Light / dark theme toggle
- Customizable wallpaper with blur and opacity controls
- Configurable device polling interval
- Persistent configuration

---

## Quick Start

### Prerequisites
- **Qt 6** (Core, Quick, Widgets, Network, WebSockets, QuickControls2, Concurrent)
- **ADB** installed and available in `PATH` ([Platform Tools](https://developer.android.com/tools/releases/platform-tools))

### Build

```bash
git clone --recursive https://github.com/mhduiy/AndroidTools-qml.git
cd AndroidTools-qml
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
```

On macOS you also need FFmpeg:
```bash
brew install ffmpeg
```

### Usage
1. Enable USB Debugging (or Wireless Debugging) on your Android device
2. Connect via USB or pair wirelessly in the app
3. Select your device from the sidebar
4. Start using any feature

---

## Roadmap

- [ ] Logcat viewer with real-time filtering
- [ ] Macro recording and playback for screen mirroring
- [ ] Magisk patch installer
- [ ] System tray battery indicator
- [ ] EXE / AppImage packaging
- [ ] Migrate UI components to FluentUI

---

## Acknowledgments

This project builds upon the work of these great projects:

- [QtScrcpy](https://github.com/barry-ran/QtScrcpy) — scrcpy integration and video decoding
- [QmlScrcpy](https://github.com/mahdi-cpp/QmlScrcpy) — QML-based scrcpy GUI reference
- [FluentUI](https://github.com/zhuzichu520/FluentUI) — Fluent Design component library for QML
- [awesome-adb](https://github.com/mzlogin/awesome-adb) — Comprehensive ADB command reference

---

## License

[MIT](LICENSE)
