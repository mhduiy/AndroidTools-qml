set(APP_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/appglobal.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/appglobal.h
)

target_sources(${EXE_NAME} PRIVATE ${APP_SRC})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)