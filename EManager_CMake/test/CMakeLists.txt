add_subdirectory(googletest)

set (GTestRootDirectory "${CMAKE_CURRENT_SOURCE_DIR}/googletest")

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE) # from googletest github

add_subdirectory(basic_tests)