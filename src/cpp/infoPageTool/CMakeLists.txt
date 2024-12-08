set(INFO_PAGE_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/adbcontrol.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/adbcontrol.h
    ${CMAKE_CURRENT_SOURCE_DIR}/batteryControl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/batteryControl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/cutactivitycontrol.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cutactivitycontrol.h
    ${CMAKE_CURRENT_SOURCE_DIR}/detailinfocontrol.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/detailinfocontrol.h
    ${CMAKE_CURRENT_SOURCE_DIR}/infopagetool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/infopagetool.h
    ${CMAKE_CURRENT_SOURCE_DIR}/realtimeinfohelper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/realtimeinfohelper.cpp
)

target_sources(${EXE_NAME} PRIVATE ${INFO_PAGE_SRC})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)