set(CONTROL_PAGE_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/batterydisguise.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/batterydisguise.h
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/controlPageTool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/controlPageTool.h
    ${CMAKE_CURRENT_SOURCE_DIR}/devicecontrol.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/devicecontrol.h
    ${CMAKE_CURRENT_SOURCE_DIR}/filetransfer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/filetransfer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/resolutioncontrol.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/resolutioncontrol.h
    ${CMAKE_CURRENT_SOURCE_DIR}/startactivity.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/startactivity.h
    ${CMAKE_CURRENT_SOURCE_DIR}/textinput.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/textinput.h
)

target_sources(${EXE_NAME} PRIVATE ${CONTROL_PAGE_SRC})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)