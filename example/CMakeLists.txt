cmake_minimum_required(VERSION 3.10)
# project(database_armory_example)

add_executable(postgres_armory_example postgres.cpp)
add_executable(sqlite_armory_example sqlite.cpp)
# target_link_libraries(${PROJECT_NAME} PRIVATE ${LIB_ALIAS})
# Link each example executable with the required libraries
target_link_libraries(postgres_armory_example PRIVATE ${LIB_ALIAS})
target_link_libraries(sqlite_armory_example PRIVATE ${LIB_ALIAS})
