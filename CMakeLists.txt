cmake_minimum_required(VERSION 3.20)

project(AndroidTools VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)

file(GLOB_RECURSE SRC_PATHS src/*.cpp src/*.hpp)
file(GLOB_RECURSE HEAD_PATHS src/*.h)
file(GLOB_RECURSE QML_PATHS src/*.qml)
file(GLOB_RECURSE RES_PATHS src/*.qrc)

# 定义需要排除的正则表达式列表
set(EXCLUDE_REGEX_LIST "src/cpp/imagePageTool/scrcpy/core/.*")

# 函数：应用排除正则表达式列表过滤路径
function(filter_paths var)
    foreach(regex IN LISTS EXCLUDE_REGEX_LIST)
        list(FILTER ${var} EXCLUDE REGEX "${regex}")
    endforeach()
    set(${var} ${${var}} PARENT_SCOPE)
endfunction()

# 对每个路径列表应用排除过滤
filter_paths(SRC_PATHS)
filter_paths(HEAD_PATHS)
filter_paths(QML_PATHS)
filter_paths(RES_PATHS)


find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Quick Widgets Gui Network Multimedia WebSockets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick Widgets Gui Network Multimedia WebSockets)

set(QSC_DEPLOY_PATH ${CMAKE_SOURCE_DIR}/.output/)



include_directories(${CMAKE_SOURCE_DIR}/.)
include_directories(${CMAKE_SOURCE_DIR}/src/cpp/imagePageTool/scrcpy)
include_directories(${CMAKE_SOURCE_DIR}/src/cpp/imagePageTool/core)
        
add_subdirectory(src/cpp/imagePageTool/scrcpy/core)

add_executable(${PROJECT_NAME}  ${QRC_SOURCES} ${SRC_PATHS} ${RES_PATHS})

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
    mfloatui
    QtScrcpyCore
    qtermwidget6
)
