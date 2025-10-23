set(LIBRARY_NAME database)

set(DATABASE_SOURCES

)

set(DATABASE_HEADERS
factory.h
)

add_subdirectory(driver/postgres/libpqxx)



add_library(${LIBRARY_NAME} STATIC ${DATABASE_SOURCES})

#define public in order we cane use library in test folder
target_link_libraries(${LIBRARY_NAME} PUBLIC pqxx )

# Keep headers associated with this target for IDEs, but do not propagate
target_sources(${LIBRARY_NAME} PRIVATE ${DATABASE_HEADERS})

target_include_directories(${LIBRARY_NAME} 
PUBLIC 
${CMAKE_CURRENT_SOURCE_DIR} 
)

# Convenient namespaced alias for downstream targets
add_library(${LIB_ALIAS} ALIAS ${LIBRARY_NAME})