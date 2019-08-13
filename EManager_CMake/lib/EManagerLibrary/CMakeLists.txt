set (LibraryName "EManagerSharedLibrary")

set (${LibraryName}IncludeDir "${CMAKE_CURRENT_SOURCE_DIR}/include")

file(GLOB HEADER_LIST "${${LibraryName}IncludeDir}/EManager/*.h")

file (GLOB SOURCES_LIST "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

set (CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_library(${LibraryName} SHARED ${SOURCES_LIST} ${HEADER_LIST})

set_target_properties(${LibraryName} PROPERTIES 
					  LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib"
					  RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

target_include_directories(${LibraryName} PUBLIC ${${LibraryName}IncludeDir})

target_compile_features(${LibraryName} PUBLIC cxx_std_17)

list(APPEND EMANAGER_INCLUDE_PATHS ${${LibraryName}IncludeDir})

set (EMANAGER_INCLUDE_PATHS ${EMANAGER_INCLUDE_PATHS} PARENT_SCOPE)

