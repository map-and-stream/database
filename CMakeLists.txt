cmake_minimum_required(VERSION 3.10)
project(database)

set(CMAKE_CXX_STANDARD 17)
set(LIB_ALIAS isiran::database)

option(LOG_BUILD_TESTS "Build log library tests" OFF)
option(LOG_BUILD_EXAMPLE "Build log example tests" OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Build the example
add_subdirectory(src)
if(DATBASE_BUILD_TESTS)
    add_subdirectory(test)
else()
    message("-- DATBASE_BUILD_TESTS is not set")
endif()
if(DATABASE_BUILD_EXAMPLE)
    add_subdirectory(example)
else()
    message("-- DATABASE_BUILD_EXAMPLE is not set")
endif()