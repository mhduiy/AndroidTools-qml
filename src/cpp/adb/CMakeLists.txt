set(ADB_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/adbinterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/adbinterface.h
    ${CMAKE_CURRENT_SOURCE_DIR}/adbtools.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/adbtools.h
    ${CMAKE_CURRENT_SOURCE_DIR}/connectmanager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/connectmanager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/keycodes.h
    ${CMAKE_CURRENT_SOURCE_DIR}/adblog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/adblog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/device.h
    ${CMAKE_CURRENT_SOURCE_DIR}/device.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/adbdevice.h
    ${CMAKE_CURRENT_SOURCE_DIR}/adbdevice.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/fastbootdevice.h
    ${CMAKE_CURRENT_SOURCE_DIR}/fastbootdevice.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/devicehelper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/devicehelper.cpp
)

target_sources(${EXE_NAME} PRIVATE ${ADB_SRC})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
