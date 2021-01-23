
cmake_minimum_required(VERSION 3.4.1)

set ( BASICS_CODE_PATH             ${CMAKE_CURRENT_LIST_DIR}/../../code )
set ( BASICS_AUDIO_HEADERS_PATH    ${BASICS_CODE_PATH}/audio/headers     )
set ( BASICS_AUDIO_SOURCES_PATH    ${BASICS_CODE_PATH}/audio/sources     )
set ( BASICS_AUDIO_ADAPTERS_PATH   ${BASICS_CODE_PATH}/audio/adapters    )

include_directories ( ${BASICS_AUDIO_HEADERS_PATH} )

file (
    GLOB_RECURSE
    BASICS_AUDIO_SOURCES
    ${BASICS_AUDIO_ADAPTERS_PATH}/android/*
    ${BASICS_AUDIO_SOURCES_PATH}/*
)

add_library (
    basics-audio
    STATIC
    ${BASICS_AUDIO_SOURCES}
)

target_link_libraries (
    basics-audio
    android
    log
    OpenSLES
)
