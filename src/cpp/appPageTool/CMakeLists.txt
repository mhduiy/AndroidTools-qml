set(APP_PAGE_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/appDetailControl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/appDetailControl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/AppPageTool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/AppPageTool.h
    ${CMAKE_CURRENT_SOURCE_DIR}/softlistmodel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/softlistmodel.h
)

target_sources(${EXE_NAME} PRIVATE ${APP_PAGE_SRC})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)