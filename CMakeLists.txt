cmake_minimum_required(VERSION 3.20)

project(AndroidTools VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)

file(GLOB_RECURSE SRC_PATHS src/*.cpp src/*.hpp)
file(GLOB_RECURSE HEAD_PATHS src/*.h)
file(GLOB_RECURSE QML_PATHS src/*.qml)
file(GLOB_RECURSE RES_PATHS src/*.qrc)

set(SRC_PATHS
    src/main.cpp
)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Quick Widgets Gui Network Multimedia WebSockets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick Widgets Gui Network Multimedia WebSockets)

set(QSC_DEPLOY_PATH ${CMAKE_SOURCE_DIR}/.output/)

include_directories(.)

add_subdirectory(${CMAKE_SOURCE_DIR}/src/cpp/adb)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/cpp/appPagetools)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/cpp/components)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/cpp/controlPageTool)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/cpp/flashPageTools)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/cpp/imagePageTool)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/cpp/infoPageTool)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/cpp/settingPageTools)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/cpp/utils)

add_executable(${PROJECT_NAME}
    ${QRC_SOURCES}
    ${SRC_PATHS}
    ${RES_PATHS}

    ${ADB_SRC}
    ${COMPONENTS_SRC}
    ${APP_PAGE_SRC}
    ${CONTROL_PAGE_SRC}
    ${FLASH_PAGE_SRC}
    ${IMAGE_PAGE_SRC}
    ${INFO_PAGE_SRC}
    ${SETTING_PAGE_SRC}
    ${UTILS_SRC}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Multimedia
    Qt${QT_VERSION_MAJOR}::WebSockets
    Qt6::GuiPrivate
    Qt6::WidgetsPrivate
    Qt6::QuickPrivate
)
