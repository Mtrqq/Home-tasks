add_executable(${CMAKE_PROJECT_NAME}_Basic_Test test.cpp)

set (GTestPath "${PROJECT_SOURCE_DIR}/lib/googletest")

message(" Path :${GTestPath}/googlemock/include")

target_include_directories(${CMAKE_PROJECT_NAME}_Basic_Test
			   PUBLIC "${GTestPath}/googletest/include"
			   PUBLIC "${GTestPath}/googlemock/include")
			  # PUBLIC "${PROJECT_SOURCE_DIR}/include") include dll ?

target_link_libraries(${CMAKE_PROJECT_NAME}_Basic_Test gmock gmock_main)

target_link_libraries(${CMAKE_PROJECT_NAME}_Basic_Test EManagerSharedLibrary)

add_test(NAME Basic_Test COMMAND ${CMAKE_PROJECT_NAME}_Basic_Test)