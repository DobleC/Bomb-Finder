
cmake_minimum_required(VERSION 3.4.1)

set ( APP_PATH  ${CMAKE_CURRENT_SOURCE_DIR}    )
set ( SRC_PATH  ${APP_PATH}/../../../code      )
set ( LIB_PATH  ${APP_PATH}/../../../libraries )

include ( ${LIB_PATH}/basics/projects/base/CMakeLists.txt     )
include ( ${LIB_PATH}/basics/projects/gaming/CMakeLists.txt   )
include ( ${LIB_PATH}/basics/projects/math/CMakeLists.txt     )
include ( ${LIB_PATH}/basics/projects/opengles/CMakeLists.txt )
include ( ${LIB_PATH}/basics/projects/png/CMakeLists.txt      )

file ( GLOB_RECURSE  SOURCES  ${SRC_PATH}/* )

add_library (
    native
    SHARED
    ${SOURCES}
)

target_link_libraries (
    native
    basics-base
    basics-opengles
    basics-gaming
    basics-png
)
