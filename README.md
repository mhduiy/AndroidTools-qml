# AndroidTools-qml

[中文介绍](README.zh_CN.md)

## One sentence introduction

A cross platform Android toolbox developed using Qt based on ADB tools, previously known as [AndroidTools](https://github.com/mhduiy/AndroidTools)The QML refactoring version can be considered as ADB's GUI tool

## Display image

![alt text](doc/image0.png)

## Why is there this project

1. Driven by interest
2. Currently, some ADB tools generally have fewer functions, and the UI is relatively ugly without animations (I personally like animations)
3. Consolidate the coding capabilities of C++and QML
4. ...

## What functions are included in the plan

#### Program framework

- [x] Customize TitleBar
- [x] Wireless connection
- [x] Multi device connection
- [x] Cross platform support

#### Equipment information

- [x] Display of basic equipment information, model, serial number, etc
- [x] Real time battery information
- [x] Front end application management
- [ ] CPU, GPU and other monitoring strips

#### Equipment control

- [x] Music control
- [x] Button simulation
- [x] Broadcast control
- [x] Electricity camouflage
- [x] Resolution and DPI modification
- [x] File transfer
- [x] Keyboard sharing
- [x] Manual activation of activity

#### Software Management

- [x] Viewing basic information of device installation software
- [x] Basic software operations, uninstallation, stopping, etc
- [x] Extract software, freeze and unfreeze (disabled function of old Android)
- [x] Start the software or increase the capacity
- [x] Install software
- [ ] Advanced installation, downgrade installation, overwrite installation, installation to location SD card, etc

#### Flashing tool

- [x] Fastboot device detection
- [x] Temporary boot up
- [x] Clear/flash specified partition
- [ ] Execution of flashing scripts (supports cross platform)
- [x] Quick flashing for Xiaomi devices
- [ ] Unpacking of line brush package/card brush package
- [x] Convert wire brush package to card brush package
- [x] Convert card brush package to wire brush package
- [x] Friend Chain
- [ ]Quick patch installation of Magisk

#### Device image

- [x] Device screen projection
- [x] Device screenshot
- [ ] Screen projection parameter control
- [ ] Simulate Click
- [ ] Recording macros

#### Setting up

- [x] Persistent storage
- [ ] Switching between dark and light colors
- [x] Blurriness and Transparency of Wallpaper
- [ ]ADB detection interval
- [ ] Element Transparency

#### Other

- [ ] Device battery icon on the system tray
- [ ] exe package support
- [ ] Deb package support
- [ ] Automated Building and Publishing of GitHub Actions

## Compile

prerequisite

- Qt6

## How to use

1. Install ADB => https://developer.android.com/tools/releases/platform-tools
2. Add the bin directory of ADB tool to the PATH environment variable
3. The device to be connected needs to enable USB debugging or wireless debugging in developer mode
4. Open this software to automatically detect device connections
5. You can start using it now

## Other Display Images

![alt text](doc/image.png)

![alt text](doc/image-1.png)

![alt text](doc/image-2.png)

![alt text](doc/image-3.png)

![alt text](doc/image-4.png)

![alt text](doc/image-5.png)

## Thank

The development of this project referred to the following projects. Thank you, experts

[QmlScrcpy]( https://github.com/mahdi-cpp/QmlScrcpy )

[QtScrcpy]( https://github.com/barry-ran/QtScrcpy )

[FluentUI]( https://github.com/zhuzichu520/FluentUI )

[awesome]( https://github.com/mzlogin/awesome-adb )