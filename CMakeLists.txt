PROJECT(raw_io)
cmake_minimum_required(VERSION 2.6)

SET (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/.orogen/config")
INCLUDE(raw_ioBase)

# Do not forget to add syskit as a test dependency in manifest.xml
#  <test_depend name="tools/syskit" />
if (ROCK_TEST_ENABLED)
    enable_testing()
    find_package(Syskit REQUIRED)
    syskit_orogen_tests(test)
endif()

# FIND_PACKAGE(KDL)
# FIND_PACKAGE(OCL)

