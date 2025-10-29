cmake_minimum_required(VERSION 3.15)
project(sqlite3 C)

add_library(sqlite3 STATIC sqlite3.c)

target_include_directories(sqlite3 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_definitions(sqlite3 PRIVATE
    SQLITE_THREADSAFE=1
    SQLITE_ENABLE_JSON1
    SQLITE_ENABLE_FTS5
    SQLITE_OMIT_LOAD_EXTENSION
)

