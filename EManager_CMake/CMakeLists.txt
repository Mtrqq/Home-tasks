cmake_minimum_required(VERSION 3.15.1)

project (EManager)

SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY
    ${PROJECT_SOURCE_DIR}/lib CACHE
    PATH "Directory where all the .lib files are dumped." FORCE)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${PROJECT_SOURCE_DIR}/bin CACHE
    PATH "Directory where .exe and .dll files are dumped." FORCE)

add_subdirectory(lib)

add_subdirectory(app)

include(CTest)

enable_testing()

add_subdirectory(tests)
