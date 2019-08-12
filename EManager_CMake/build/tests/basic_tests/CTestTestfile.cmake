# CMake generated Testfile for 
# Source directory: C:/Users/Antoshik/source/repos/HomeTasks/EManager_CMake/tests/basic_tests
# Build directory: C:/Users/Antoshik/source/repos/HomeTasks/EManager_CMake/build/tests/basic_tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(Basic_Test "C:/Users/Antoshik/source/repos/HomeTasks/EManager_CMake/bin/Debug/EManager_Basic_Test.exe")
  set_tests_properties(Basic_Test PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/Antoshik/source/repos/HomeTasks/EManager_CMake/tests/basic_tests/CMakeLists.txt;16;add_test;C:/Users/Antoshik/source/repos/HomeTasks/EManager_CMake/tests/basic_tests/CMakeLists.txt;0;")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(Basic_Test "C:/Users/Antoshik/source/repos/HomeTasks/EManager_CMake/bin/Release/EManager_Basic_Test.exe")
  set_tests_properties(Basic_Test PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/Antoshik/source/repos/HomeTasks/EManager_CMake/tests/basic_tests/CMakeLists.txt;16;add_test;C:/Users/Antoshik/source/repos/HomeTasks/EManager_CMake/tests/basic_tests/CMakeLists.txt;0;")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test(Basic_Test "C:/Users/Antoshik/source/repos/HomeTasks/EManager_CMake/bin/MinSizeRel/EManager_Basic_Test.exe")
  set_tests_properties(Basic_Test PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/Antoshik/source/repos/HomeTasks/EManager_CMake/tests/basic_tests/CMakeLists.txt;16;add_test;C:/Users/Antoshik/source/repos/HomeTasks/EManager_CMake/tests/basic_tests/CMakeLists.txt;0;")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test(Basic_Test "C:/Users/Antoshik/source/repos/HomeTasks/EManager_CMake/bin/RelWithDebInfo/EManager_Basic_Test.exe")
  set_tests_properties(Basic_Test PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/Antoshik/source/repos/HomeTasks/EManager_CMake/tests/basic_tests/CMakeLists.txt;16;add_test;C:/Users/Antoshik/source/repos/HomeTasks/EManager_CMake/tests/basic_tests/CMakeLists.txt;0;")
else()
  add_test(Basic_Test NOT_AVAILABLE)
endif()
