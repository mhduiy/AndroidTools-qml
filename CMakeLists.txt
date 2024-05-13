cmake_minimum_required(VERSION 3.20)

project(AndroidTools VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)

file(GLOB_RECURSE SRC_PATHS src/*.cpp src/*.hpp)
file(GLOB_RECURSE HEAD_PATHS src/*.h)
file(GLOB_RECURSE QML_PATHS src/*.qml)
file(GLOB_RECURSE RES_PATHS src/*.qrc)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Quick)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick)

include_directories(.)

add_executable(${PROJECT_NAME}  ${QRC_SOURCES} ${SRC_PATHS} ${RES_PATHS})

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Quick
    mfloatui
)
