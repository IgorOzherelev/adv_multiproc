cmake_minimum_required(VERSION 3.23)
project(bst)

include(FetchContent)
FetchContent_Declare(parlaylib
        GIT_REPOSITORY  https://github.com/cmuparlay/parlaylib.git
        GIT_TAG         master
)
FetchContent_GetProperties(parlaylib)
if(NOT parlaylib_POPULATED)
    FetchContent_Populate(parlaylib)
    add_subdirectory(${parlaylib_SOURCE_DIR} ${parlaylib_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

file (GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*")
file (GLOB_RECURSE IMPL CONFIGURE_DEPENDS "impl/*")

set(CMAKE_CXX_STANDARD 23)

add_executable(bst main.cpp ${SOURCES} ${IMPL})
target_link_libraries(bst PRIVATE parlay)