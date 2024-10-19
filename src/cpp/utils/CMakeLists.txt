# 添加源文件
set(UTILS_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/constants.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/constants.h
    ${CMAKE_CURRENT_SOURCE_DIR}/globalsetting.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/globalsetting.h
    ${CMAKE_CURRENT_SOURCE_DIR}/keyconvert.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/keyconvert.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Notification.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Notification.h
    ${CMAKE_CURRENT_SOURCE_DIR}/singleton.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/utils.hpp
    PARENT_SCOPE
)

# 添加头文件路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)