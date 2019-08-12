
add_executable(${PROJECT_NAME}_Application src/main.cpp)

target_link_libraries(${PROJECT_NAME}_Application PRIVATE EManagerSharedLibrary)