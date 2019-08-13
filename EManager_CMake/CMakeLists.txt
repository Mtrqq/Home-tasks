cmake_minimum_required(VERSION 3.15.1)

project (EManager VERSION 1.0.0 LANGUAGES CXX)

add_subdirectory(lib)

add_subdirectory(app)

enable_testing()

add_subdirectory(test)
