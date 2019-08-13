add_executable(${PROJECT_NAME}_Application src/main.cpp)

set_target_properties(${PROJECT_NAME}_Application PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

target_include_directories(${PROJECT_NAME}_Application PUBLIC ${EMANAGER_INCLUDE_PATHS})

target_link_libraries(${PROJECT_NAME}_Application PRIVATE EManagerSharedLibrary)