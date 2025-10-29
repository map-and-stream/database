set(LIBRARY_NAME database)

set(DATABASE_SOURCES
postgresql.cpp
sqlite.cpp
)

set(DATABASE_HEADERS
postgresql.h
factory.h
config.h
database.h
sqlite.h
query_result.h
querybuilder/query_builder.h
)

add_subdirectory(driver/postgres/libpqxx)
add_subdirectory(sqlite)

# Ensure pthread is linked
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_library(${LIBRARY_NAME} STATIC ${DATABASE_SOURCES})

#define public in order we cane use library in test folder
target_link_libraries(${LIBRARY_NAME} PUBLIC pqxx sqlite3 Threads::Threads)

# Keep headers associated with this target for IDEs, but do not propagate
target_sources(${LIBRARY_NAME} PRIVATE ${DATABASE_HEADERS})

target_include_directories(${LIBRARY_NAME} 
PUBLIC 
${CMAKE_CURRENT_SOURCE_DIR} 
)

# Convenient namespaced alias for downstream targets
add_library(${LIB_ALIAS} ALIAS ${LIBRARY_NAME})
