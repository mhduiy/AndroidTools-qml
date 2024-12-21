set(FLASH_PAGE_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/fastbootdevicemanager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/fastbootdevicemanager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/flashPageTool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/flashPageTool.h
    ${CMAKE_CURRENT_SOURCE_DIR}/flashtools.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/flashtools.h
    ${CMAKE_CURRENT_SOURCE_DIR}/flashUrlHandle.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/flashUrlHandle.h
)

target_sources(${EXE_NAME} PRIVATE ${FLASH_PAGE_SRC})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)