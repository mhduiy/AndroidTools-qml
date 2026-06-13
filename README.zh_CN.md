# AndroidTools-qml

AndroidTools-qml 是一个基于 Qt 6、QML、FluentUI、ADB/Fastboot 和 scrcpy 的跨平台 Android 工具箱。它把设备信息、投屏、控制、应用管理、文件传输、日志和刷机工具整合到一个紧凑的桌面控制台里。

这是旧版 [AndroidTools](https://github.com/mhduiy/AndroidTools) 的 QML 重构版本。

![AndroidTools 控制台](doc/image.png)

## 核心特性

- 信息密度更高的单页控制台：设备档案、投屏舞台、实时状态、快捷工作台集中显示。
- 接入设备侧服务，展示 CPU、GPU、内存、存储、电池、前台应用、进程和 FPS 等实时数据。
- 实时状态可视化：小仪表盘、资源趋势折线图、前台进程和投屏状态。
- 应用管理：真实应用图标、应用列表、包名、版本、安装时间、SDK 信息和常用操作。
- 投屏能力：启动/停止投屏、截图、FPS/码率质量预设、基础按键控制。
- 设备控制：主页、返回、电源、菜单、音量、静音、重启、媒体控制。
- 调试工具：电池伪装、分辨率/DPI 修改、文件推送、键盘输入转发、Activity 启动。
- 刷机工具：Fastboot 设备管理、临时启动镜像、刷入/擦除分区、维护操作入口。
- 设置：深浅色切换、壁纸、模糊和透明度等持久化配置。

## 功能概览

### 控制台

- 设备基础信息：厂商、型号、Android 版本、分辨率、DPI、CPU、内存、电池、IP 和序列号。
- 实时状态：CPU/GPU/RAM/温度/FPS 趋势。
- 当前上下文：前台包名、前台 Activity、进程信息、投屏状态。
- 底部工作台：控制、快捷入口、文件与输入、应用、刷机、日志。

### 应用管理

- 按第三方、系统、全部筛选应用。
- 显示应用真实图标、名称、包名和版本。
- 支持安装 APK、启动、停止、提取、冻结、清数据和卸载。
- 展示版本、版本码、安装时间、最低 SDK 和目标 SDK。

### 投屏与控制

- 基于 scrcpy 的设备投屏。
- 支持截图和质量预设。
- 支持主页、返回、电源、菜单、音量、静音、重启等快捷按键。

### 文件、输入与调试

- 文件推送到设备路径。
- PC 键盘输入转发到设备。
- 手动启动 Activity。
- 电池状态伪装和分辨率/DPI 修改。

### 刷机工具

- Fastboot 设备刷新。
- 重启到系统、Fastboot、Recovery 或关机。
- 临时启动镜像。
- 刷入或擦除指定分区。

## 环境要求

- Qt 6：Core、Quick、Widgets、Gui、Network、WebSockets、QuickControls2、Concurrent。
- CMake 3.20 或更新版本。
- ADB/Fastboot 已安装，并加入 `PATH`。
- Android 设备开启 USB 调试或无线调试。
- macOS 投屏相关依赖建议安装 FFmpeg：

```bash
brew install ffmpeg
```

## 编译

```bash
git clone --recursive https://github.com/mhduiy/AndroidTools-qml.git
cd AndroidTools-qml
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
```

如果没有拉取子模块：

```bash
git submodule update --init --recursive
```

## 使用

1. 安装 Android Platform Tools，确保 `adb` 和 `fastboot` 在 `PATH` 中。
2. 在 Android 设备上开启 USB 调试或无线调试。
3. 启动 AndroidTools。
4. 连接设备，应用会在需要时自动启动设备侧辅助服务。
5. 在控制台中进行投屏、设备控制、应用管理、文件传输、刷机和日志查看。

## 项目结构

```text
src/cpp/adb                 ADB 设备模型、命令执行、设备侧服务桥接
src/cpp/controlPageTool     设备控制、电池伪装、文件和输入工具
src/cpp/AppPageTool         应用列表、应用详情、图标加载、应用操作
src/cpp/flashPageTool       Fastboot 和刷机工具
src/cpp/components          实时系统信息提供器
src/cpp/imagePageTool       scrcpy / 投屏集成
src/qml2                    QML 用户界面
doc                         截图和文档资源
```

## 致谢

- [QtScrcpy](https://github.com/barry-ran/QtScrcpy)
- [QmlScrcpy](https://github.com/mahdi-cpp/QmlScrcpy)
- [FluentUI](https://github.com/zhuzichu520/FluentUI)
- [awesome-adb](https://github.com/mzlogin/awesome-adb)

## 许可证

[MIT](LICENSE)
