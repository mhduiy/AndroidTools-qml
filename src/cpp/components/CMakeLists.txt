set(COMPONENTS_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/fpsitem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/fpsitem.h
    ${CMAKE_CURRENT_SOURCE_DIR}/SystemInfoProvider.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SystemInfoProvider.h
)

target_sources(${EXE_NAME} PRIVATE ${COMPONENTS_SRC})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)