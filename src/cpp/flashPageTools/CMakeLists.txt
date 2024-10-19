# 添加源文件
set(FLASH_PAGE_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/fastbootdevicemanager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/fastbootdevicemanager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/flashPageTool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/flashPageTool.h
    ${CMAKE_CURRENT_SOURCE_DIR}/flashtools.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/flashtools.h
    ${CMAKE_CURRENT_SOURCE_DIR}/flashUrlHandle.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/flashUrlHandle.h
    PARENT_SCOPE
)

# 添加头文件路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)