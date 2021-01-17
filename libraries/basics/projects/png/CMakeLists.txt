
cmake_minimum_required(VERSION 3.4.1)

set ( BASICS_CODE_PATH          ${CMAKE_CURRENT_LIST_DIR}/../../code )
set ( BASICS_PNG_HEADERS_PATH   ${BASICS_CODE_PATH}/png/headers      )
set ( BASICS_PNG_SOURCES_PATH   ${BASICS_CODE_PATH}/png/sources      )

include_directories ( ${BASICS_PNG_HEADERS_PATH} )

file (
    GLOB_RECURSE
    BASICS_PNG_SOURCES
    ${BASICS_PNG_SOURCES_PATH}/*
)

add_library (
    basics-png
    STATIC
    ${BASICS_PNG_SOURCES}
)
