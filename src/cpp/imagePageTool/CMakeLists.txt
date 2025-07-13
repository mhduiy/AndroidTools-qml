set(IMAGE_PAGE_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/imagePageTool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imagePageTool.h
)

# MFloat
file(GLOB_RECURSE SCRCPY_SRC ${CMAKE_CURRENT_SOURCE_DIR}/scrcpy/service/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/scrcpy/ui/*.cpp)

target_sources(${EXE_NAME} PRIVATE ${IMAGE_PAGE_SRC} ${SCRCPY_SRC})
target_include_directories(${EXE_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/scrcpy)

set(QSC_DEPLOY_PATH ${CMAKE_BINARY_DIR}/lib)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/scrcpy/core)

# 链接QtScrcpyCore库
target_link_libraries(${EXE_NAME} PRIVATE QtScrcpyCore)
