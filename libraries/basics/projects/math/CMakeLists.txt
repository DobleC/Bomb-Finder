
cmake_minimum_required(VERSION 3.4.1)

set ( BASICS_CODE_PATH           ${CMAKE_CURRENT_LIST_DIR}/../../code )
set ( BASICS_MATH_HEADERS_PATH   ${BASICS_CODE_PATH}/math/headers     )
#set ( BASICS_MATH_SOURCES_PATH   ${BASICS_CODE_PATH}/math/sources     )
#set ( BASICS_MATH_ADAPTERS_PATH  ${BASICS_CODE_PATH}/math/adapters    )

include_directories ( ${BASICS_MATH_HEADERS_PATH} )

#file (
#    GLOB_RECURSE
#    BASICS_GAMING_SOURCES
#    ${BASICS_GAMING_ADAPTERS_PATH}/android/*
#    ${BASICS_GAMING_SOURCES_PATH}/*
#)

#add_library (
#    basics-gaming
#    STATIC
#    ${BASICS_GAMING_SOURCES}
#)

#target_link_libraries (
#    basics-gaming
#    EGL
#    GLESv2
#)
