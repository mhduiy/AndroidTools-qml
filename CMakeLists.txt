cmake_minimum_required(VERSION 3.20)

project(AndroidTools VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)

set(EXE_NAME ${PROJECT_NAME})

file(GLOB_RECURSE SRC_PATHS src/cpp/*.cpp src/cpp/*.hpp)
file(GLOB_RECURSE HEAD_PATHS src/cpp/*.h)
file(GLOB_RECURSE QML_PATHS src/qml/*.qml)

set(RES_PATHS
    src/main.qrc
    ${CMAKE_SOURCE_DIR}/src/MFloat/mfloat.qrc
)

set(SRC_PATHS
    src/main.cpp
)

set(QT_VERSION_MAJOR 6)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick Widgets Gui Network WebSockets QuickControls2)

include_directories(.)

add_executable(${EXE_NAME}
    ${QRC_SOURCES}
    ${SRC_PATHS}
    ${RES_PATHS}
)

add_subdirectory(${CMAKE_SOURCE_DIR}/src/cpp/adb)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/cpp/app)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/cpp/appPageTool)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/cpp/components)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/cpp/controlPageTool)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/cpp/flashPageTool)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/cpp/imagePageTool)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/cpp/infoPageTool)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/cpp/settingPageTool)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/cpp/utils)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Enable Qml Debug")
    target_compile_definitions(${EXE_NAME} PRIVATE QT_QML_DEBUG)
endif()


# MFloat
file(GLOB_RECURSE QML_PATHS ${CMAKE_SOURCE_DIR}/src/MFloat/*.qml)

foreach(QML_FILE ${QML_PATHS})
    get_filename_component(file_name ${QML_FILE} NAME)
    set_source_files_properties(${QML_FILE}
        PROPERTIES QT_RESOURCE_ALIAS ${file_name}
    )
endforeach()

set(MFloat_QRC_FILE_LIST
    ${CMAKE_SOURCE_DIR}/src/MFloat/mfloat.qrc
)

foreach(MFloat_QRC_FILE ${MFloat_QRC_FILE_LIST})
    get_filename_component(file_name ${MFloat_QRC_FILE} NAME)
    set_source_files_properties(${MFloat_QRC_FILE}
        PROPERTIES QT_RESOURCE_ALIAS ${file_name}
    )
endforeach()

qt_add_qml_module(${EXE_NAME}
    URI MFloat
    VERSION 1.0
    RESOURCES ${CMAKE_SOURCE_DIR}/src/MFloat/mfloat.qrc
    RESOURCE_PREFIX
        /qt/qml
    QML_FILES
        ${QML_PATHS}
)

target_link_libraries(${EXE_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::WebSockets
    Qt${QT_VERSION_MAJOR}::QuickControls2

    QtScrcpyCore
)
