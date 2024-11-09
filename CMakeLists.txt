cmake_minimum_required(VERSION 3.20)

project(AndroidTools VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)

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

add_subdirectory(${CMAKE_SOURCE_DIR}/src/cpp/adb)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/cpp/appPageTools)
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
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE QT_QML_DEBUG)
endif()


# 遍历所有qml文件
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

qt_add_qml_module(${PROJECT_NAME}
    URI MFloat
    VERSION 1.0
    RESOURCES ${CMAKE_SOURCE_DIR}/src/MFloat/mfloat.qrc
    RESOURCE_PREFIX
        /qt/qml
    QML_FILES
        ${QML_PATHS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::WebSockets
    Qt${QT_VERSION_MAJOR}::QuickControls2
)
