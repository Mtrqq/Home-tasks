add_executable(${PROJECT_NAME}_Basic_Test src/test.cpp)

set_target_properties(${PROJECT_NAME}_Basic_Test PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

target_include_directories(${PROJECT_NAME}_Basic_Test
			   PUBLIC "${GTestRootDirectory}/googletest/include"
			   PUBLIC "${GTestRootDirectory}/googlemock/include"
			   PUBLIC "${EMANAGER_INCLUDE_PATHS}")

target_link_libraries(${PROJECT_NAME}_Basic_Test gmock gmock_main EManagerSharedLibrary)

add_test(NAME Basic_Test COMMAND ${PROJECT_NAME}_Basic_Test)