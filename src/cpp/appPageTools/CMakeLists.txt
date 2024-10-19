# 添加源文件
set(APP_PAGE_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/appDetailControl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/appDetailControl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/appHelper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/appHelper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/appPagetool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/appPagetool.h
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/softlistmodel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/softlistmodel.h
    PARENT_SCOPE
)

# 添加头文件路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)