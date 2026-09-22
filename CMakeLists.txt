cmake_minimum_required(VERSION 3.20)

project(AndroidTools VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTORCC ON)
include(GNUInstallDirs)
set(CMAKE_AUTOMOC ON)

set(EXE_NAME ${PROJECT_NAME})

set(SRC_PATHS
    src/main.cpp
)

find_package(Qt6 REQUIRED COMPONENTS Core Quick Widgets Gui Network WebSockets QuickControls2 Concurrent DBus ShaderTools)

add_executable(${EXE_NAME}
    ${SRC_PATHS}
    src/main.qrc
)

target_include_directories(${EXE_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/third_party/FluentUI/CMakeLists.txt")
    message(FATAL_ERROR "FluentUI submodule is missing; run git submodule update --init --recursive")
endif()

set(FLUENTUI_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(FLUENTUI_QML_PLUGIN_DIRECTORY "${CMAKE_BINARY_DIR}/qml/FluentUI" CACHE PATH "" FORCE)
add_subdirectory(third_party/FluentUI EXCLUDE_FROM_ALL)
add_dependencies(${EXE_NAME} fluentuiplugin)

if(APPLE)
    set_target_properties(${EXE_NAME} PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_GUI_IDENTIFIER com.mhduiy.androidtools
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION}
    )
endif()

add_subdirectory(${CMAKE_SOURCE_DIR}/src/cpp/adb)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/cpp/app)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/cpp/utils)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/cpp/components)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/cpp/settingPageTool)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/cpp/controlPageTool)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/cpp/appPageTool)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/cpp/flashPageTool)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/cpp/imagePageTool)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Enable Qml Debug")
    target_compile_definitions(${EXE_NAME} PRIVATE QT_QML_DEBUG)
endif()

target_link_libraries(${EXE_NAME} PRIVATE
    Qt6::Core
    Qt6::DBus
    Qt6::Quick
    Qt6::Widgets
    Qt6::Gui
    Qt6::Network
    Qt6::WebSockets
    Qt6::QuickControls2
    Qt6::Concurrent
    Qt6::ShaderTools
)

set(BUNDLED_ADB_DIR "${CMAKE_SOURCE_DIR}/src/cpp/imagePageTool/scrcpy/core/src/third_party/adb")
if(APPLE)
    set(BUNDLED_ADB "${BUNDLED_ADB_DIR}/mac/adb")
    add_custom_command(TARGET ${EXE_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_BUNDLE_DIR:${EXE_NAME}>/Contents/Resources/qml"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${FLUENTUI_QML_PLUGIN_DIRECTORY}" "$<TARGET_BUNDLE_DIR:${EXE_NAME}>/Contents/Resources/qml/FluentUI"
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_BUNDLE_DIR:${EXE_NAME}>/Contents/Resources/tools"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${BUNDLED_ADB}" "$<TARGET_BUNDLE_DIR:${EXE_NAME}>/Contents/Resources/tools/adb"
    )
elseif(UNIX)
    set(BUNDLED_ADB "${BUNDLED_ADB_DIR}/linux/adb")
endif()

install(TARGETS ${EXE_NAME}
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(UNIX AND NOT APPLE)
    install(PROGRAMS "${BUNDLED_ADB}" DESTINATION ${CMAKE_INSTALL_LIBDIR}/android-tools/tools)
    install(DIRECTORY "${FLUENTUI_QML_PLUGIN_DIRECTORY}/" DESTINATION ${CMAKE_INSTALL_DATADIR}/android-tools/qml/FluentUI)
    install(FILES packaging/android-tools.desktop DESTINATION ${CMAKE_INSTALL_DATADIR}/applications)
endif()

install(FILES LICENSE DESTINATION ${CMAKE_INSTALL_DATADIR}/licenses/android-tools)

set(CPACK_PACKAGE_NAME "android-tools")
set(CPACK_PACKAGE_VENDOR "mhduiy")
set(CPACK_PACKAGE_CONTACT "mhduiy@qq.com")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Qt toolbox for Android devices")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}" CACHE STRING "Package version")
if(UNIX AND NOT APPLE)
    set(CPACK_GENERATOR "DEB")
    set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
    set(CPACK_DEBIAN_PACKAGE_SECTION utils)
    set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "adb, fastboot, qml6-module-qt5compat-graphicaleffects, qml6-module-qtquick-controls, qml6-module-qtquick-dialogs, qml6-module-qtquick-layouts, qml6-module-qtquick-window")
endif()
include(CPack)
