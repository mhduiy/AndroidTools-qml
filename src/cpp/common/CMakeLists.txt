set(COMMON_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/basepagetool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/basepagetool.h
    ${CMAKE_CURRENT_SOURCE_DIR}/pagetoolmanager.h
)

target_sources(${EXE_NAME} PRIVATE ${COMMON_SRC}) 