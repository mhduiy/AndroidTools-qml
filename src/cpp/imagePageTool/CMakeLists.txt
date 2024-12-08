set(IMAGE_PAGE_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/imagePageTool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imagePageTool.h
)

target_sources(${EXE_NAME} PRIVATE ${IMAGE_PAGE_SRC})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)