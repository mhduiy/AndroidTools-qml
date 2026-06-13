cmake_minimum_required(VERSION 3.20)

project(AndroidTools VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)

set(EXE_NAME ${PROJECT_NAME})

file(GLOB_RECURSE SRC_PATHS src/cpp/*.cpp src/cpp/*.hpp)
file(GLOB_RECURSE HEAD_PATHS src/cpp/*.h)
file(GLOB_RECURSE QML_PATHS src/qml2/*.qml)

set(RES_PATHS
    src/main.qrc
)

set(SRC_PATHS
    src/main.cpp
)

set(QT_VERSION_MAJOR 6)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick Widgets Gui Network WebSockets QuickControls2 Concurrent)

include_directories(.)

add_executable(${EXE_NAME}
    ${QRC_SOURCES}
    ${SRC_PATHS}
    ${QML_PATHS}
    ${RES_PATHS}
)

add_subdirectory(${CMAKE_SOURCE_DIR}/src/cpp/adb)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/cpp/app)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/cpp/utils)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/cpp/components)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/cpp/settingPageTool)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/cpp/controlPageTool)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/cpp/AppPageTool)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/cpp/flashPageTool)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/cpp/imagePageTool)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Enable Qml Debug")
    target_compile_definitions(${EXE_NAME} PRIVATE QT_QML_DEBUG)
endif()

target_link_libraries(${EXE_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::WebSockets
    Qt${QT_VERSION_MAJOR}::QuickControls2
    Qt${QT_VERSION_MAJOR}::Concurrent

    # QtScrcpyCore
)

install(TARGETS ${EXE_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/qml2 DESTINATION share/${PROJECT_NAME})
install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/res DESTINATION share/${PROJECT_NAME})
