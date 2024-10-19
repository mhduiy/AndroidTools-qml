# 添加源文件
set(SETTING_PAGE_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/bingwallpaperhander.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bingwallpaperhander.h
    ${CMAKE_CURRENT_SOURCE_DIR}/settingPageTools.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/settingPageTools.h
    ${CMAKE_CURRENT_SOURCE_DIR}/wallpaperhelper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/wallpaperhelper.h
    PARENT_SCOPE
)

# 添加头文件路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)