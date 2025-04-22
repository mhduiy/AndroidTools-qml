set(IMAGE_PAGE_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/imagePageTool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imagePageTool.h
)

# MFloat
file(GLOB_RECURSE SCRCPY_SRC ${CMAKE_CURRENT_SOURCE_DIR}/scrcpy/service/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/scrcpy/ui/*.cpp)

target_sources(${EXE_NAME} PRIVATE ${IMAGE_PAGE_SRC} ${SCRCPY_SRC})
target_include_directories(${EXE_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/scrcpy)

set(QSC_DEPLOY_PATH ${CMAKE_BINARY_DIR}/lib)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/scrcpy/core)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    # ffmpeg
    set(FFMPEG_PATH "F:\\Soft\\libffmpeg_7.1_msvc17_x64")
    target_include_directories(${EXE_NAME} PRIVATE ${FFMPEG_PATH}\\include)
    target_link_directories(${EXE_NAME} PRIVATE ${FFMPEG_PATH}\\lib)
endif()

find_program(ADB_PATH adb)
if(ADB_PATH)
    message(STATUS "Found adb at: ${ADB_PATH}")
    target_compile_definitions(${EXE_NAME} PRIVATE ADB_PATH="${ADB_PATH}")
else()
    message(FATAL_ERROR "adb not found on your system")
endif()

# MacOS
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(FFMPEG REQUIRED libswscale)

    target_include_directories(${EXE_NAME} PRIVATE ${FFMPEG_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/libyuv/include)
    target_link_directories(${EXE_NAME} PRIVATE ${FFMPEG_LIBRARY_DIRS} ${CMAKE_SOURCE_DIR}/libyuv/build)

    target_link_libraries(${EXE_NAME} PRIVATE ${FFMPEG_LIBRARIES} yuv)
endif()
