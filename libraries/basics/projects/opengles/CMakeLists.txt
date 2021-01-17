
cmake_minimum_required(VERSION 3.4.1)

set ( BASICS_CODE_PATH               ${CMAKE_CURRENT_LIST_DIR}/../../code  )
set ( BASICS_OPENGLES_HEADERS_PATH   ${BASICS_CODE_PATH}/opengles/headers  )
set ( BASICS_OPENGLES_SOURCES_PATH   ${BASICS_CODE_PATH}/opengles/sources  )
set ( BASICS_OPENGLES_ADAPTERS_PATH  ${BASICS_CODE_PATH}/opengles/adapters )

include_directories ( ${BASICS_OPENGLES_HEADERS_PATH} )

file (
    GLOB_RECURSE
    BASICS_OPENGLES_SOURCES
    ${BASICS_OPENGLES_ADAPTERS_PATH}/android/*
    ${BASICS_OPENGLES_SOURCES_PATH}/*
)

add_library (
    basics-opengles
    STATIC
    ${BASICS_OPENGLES_SOURCES}
)

target_link_libraries (
    basics-opengles
    EGL
    GLESv2
)
