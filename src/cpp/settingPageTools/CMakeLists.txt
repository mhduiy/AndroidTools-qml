set(SETTING_PAGE_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/bingwallpaperhander.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bingwallpaperhander.h
    ${CMAKE_CURRENT_SOURCE_DIR}/settingPageTools.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/settingPageTools.h
    ${CMAKE_CURRENT_SOURCE_DIR}/wallpaperhelper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/wallpaperhelper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/othersettingshandler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/othersettingshandler.h
)

target_sources(${EXE_NAME} PRIVATE ${SETTING_PAGE_SRC})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)