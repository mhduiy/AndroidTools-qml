# 添加源文件
set(ADB_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/adbinterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/adbinterface.h
    ${CMAKE_CURRENT_SOURCE_DIR}/adbtools.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/adbtools.h
    ${CMAKE_CURRENT_SOURCE_DIR}/connectmanager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/connectmanager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/keycodes.h
    PARENT_SCOPE
)

# 添加头文件路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)