
cmake_minimum_required(VERSION 3.4.1)

set ( BASICS_CODE_PATH            ${CMAKE_CURRENT_LIST_DIR}/../../code )
set ( BASICS_BASE_HEADERS_PATH    ${BASICS_CODE_PATH}/base/headers     )
set ( BASICS_BASE_SOURCES_PATH    ${BASICS_CODE_PATH}/base/sources     )
set ( BASICS_BASE_ADAPTERS_PATH   ${BASICS_CODE_PATH}/base/adapters    )

set ( CMAKE_SHARED_LINKER_FLAGS  "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate" )
set ( CMAKE_SHARED_LINKER_FLAGS  "${CMAKE_SHARED_LINKER_FLAGS} -u basics::Renderer" )
set ( CMAKE_SHARED_LINKER_FLAGS  "${CMAKE_SHARED_LINKER_FLAGS} -u basics::Window::can_be_instantiated")

include_directories ( ${BASICS_BASE_HEADERS_PATH} )

file (
    GLOB_RECURSE
    BASICS_BASE_SOURCES
    ${BASICS_BASE_ADAPTERS_PATH}/android/*
    ${BASICS_BASE_SOURCES_PATH}/*
)

add_library (
    basics-base
    STATIC
    ${BASICS_BASE_SOURCES}
)

target_link_libraries (
    basics-base
    android
    log
)
