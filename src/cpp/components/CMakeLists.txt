# 添加源文件
set(COMPONENTS_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/deivicelistviewmodel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/deivicelistviewmodel.h
    PARENT_SCOPE
)

# 添加头文件路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)