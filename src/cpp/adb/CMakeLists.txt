set(ADB_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/adbinterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/adbinterface.h
    ${CMAKE_CURRENT_SOURCE_DIR}/adbtools.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/adbtools.h
    ${CMAKE_CURRENT_SOURCE_DIR}/connectmanager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/connectmanager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/keycodes.h
    ${CMAKE_CURRENT_SOURCE_DIR}/adblog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/adblog.cpp
)

target_sources(${EXE_NAME} PRIVATE ${ADB_SRC})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
