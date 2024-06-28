cmake_minimum_required(VERSION 3.20)

project(AndroidTools VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)

file(GLOB_RECURSE SRC_PATHS src/*.cpp src/*.hpp)
file(GLOB_RECURSE HEAD_PATHS src/*.h)
file(GLOB_RECURSE QML_PATHS src/*.qml)
file(GLOB_RECURSE RES_PATHS src/*.qrc)

# 移除src/cpp/imagePageTool/scrcpy/core目录下的所有文件
set(EXCLUDE_REGEX "src/cpp/imagePageTool/scrcpy/core/.*")
list(FILTER SRC_PATHS EXCLUDE REGEX "${EXCLUDE_REGEX}")
list(FILTER HEAD_PATHS EXCLUDE REGEX "${EXCLUDE_REGEX}")
list(FILTER QML_PATHS EXCLUDE REGEX "${EXCLUDE_REGEX}")
list(FILTER RES_PATHS EXCLUDE REGEX "${EXCLUDE_REGEX}")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Quick Widgets Gui Network Multimedia WebSockets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick Widgets Gui Network Multimedia WebSockets)

set(QSC_DEPLOY_PATH ${CMAKE_SOURCE_DIR}/.output/)



include_directories(${CMAKE_SOURCE_DIR}/.)
include_directories(${CMAKE_SOURCE_DIR}/src/cpp/imagePageTool/scrcpy)
include_directories(${CMAKE_SOURCE_DIR}/src/cpp/imagePageTool/core)
include_directories(${CMAKE_SOURCE_DIR}/src/cpp/terminalPageTool/qTermWidget6/lib)

add_subdirectory(src/cpp/imagePageTool/scrcpy/core)
add_subdirectory(src/cpp/terminalPageTool/QTermWidget6)

add_executable(${PROJECT_NAME}  ${QRC_SOURCES} ${SRC_PATHS} ${RES_PATHS})

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Multimedia
    Qt${QT_VERSION_MAJOR}::WebSockets
    mfloatui
    QtScrcpyCore
)
