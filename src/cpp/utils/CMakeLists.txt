set(UTILS_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/constants.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/constants.h
    ${CMAKE_CURRENT_SOURCE_DIR}/globalsetting.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/globalsetting.h
    ${CMAKE_CURRENT_SOURCE_DIR}/keyconvert.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/keyconvert.h
    ${CMAKE_CURRENT_SOURCE_DIR}/notificationcontroller.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/notificationcontroller.h
    ${CMAKE_CURRENT_SOURCE_DIR}/defutils.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/utils.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/taskthread.h
    ${CMAKE_CURRENT_SOURCE_DIR}/taskthread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/serviceregistry.h
    ${CMAKE_CURRENT_SOURCE_DIR}/serviceregistry.cpp
)

target_sources(${EXE_NAME} PRIVATE ${UTILS_SRC})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})