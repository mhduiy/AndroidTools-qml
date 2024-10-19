# 添加源文件
set(IMAGE_PAGE_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/imagePageTool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imagePageTool.h
    PARENT_SCOPE
)

# 添加头文件路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)