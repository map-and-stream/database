# Add GoogleTest
add_subdirectory(googletest)

# Enable testing
enable_testing()

# Create your test target
add_executable(log_test
    test_log.cpp
)

# Link against GoogleTest and pthread
target_link_libraries(log_test 
PRIVATE 
    ${LIB_ALIAS}
    GTest::gtest
    GTest::gtest_main
    pthread
)
