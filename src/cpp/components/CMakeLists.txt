set(COMPONENTS_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/deivicelistviewmodel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/deivicelistviewmodel.h
)

target_sources(${EXE_NAME} PRIVATE ${COMPONENTS_SRC})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)