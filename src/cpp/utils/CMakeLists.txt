set(UTILS_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/constants.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/constants.h
    ${CMAKE_CURRENT_SOURCE_DIR}/globalsetting.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/globalsetting.h
    ${CMAKE_CURRENT_SOURCE_DIR}/keyconvert.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/keyconvert.h
    ${CMAKE_CURRENT_SOURCE_DIR}/notificationcontroller.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/notificationcontroller.h
    ${CMAKE_CURRENT_SOURCE_DIR}/singleton.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/utils.hpp
)

target_sources(${EXE_NAME} PRIVATE ${UTILS_SRC})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)