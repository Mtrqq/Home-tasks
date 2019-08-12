set (IncludeDirectoryPath "${PROJECT_SOURCE_DIR}/include")

file(GLOB HEADER_LIST "${IncludeDirectoryPath}/EManager/*.h")

file (GLOB SOURCES_LIST *.cpp)

set (CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_library("${PROJECT_NAME}Library" SHARED ${SOURCES_LIST} ${HEADER_LIST})

#set_target_properties("${PROJECT_NAME}Library" PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

#message("Path : ${PROJECT_BINARY_DIR}/bin")

#message ("FLAG : ${${PROJECT_NAME}Library_LIBRARY_OUTPUT_DIRECTORY}")

target_include_directories("${PROJECT_NAME}Library" PUBLIC ${IncludeDirectoryPath})

target_compile_features("${PROJECT_NAME}Library" PUBLIC cxx_std_17)