cmake_minimum_required (VERSION 3.0)
project (BUILD_LIBRARIES VERSION 1.0)

#Libs don't really need c++11?
#Causes build failure for googletest cause of strict ansi settings c++11 flag adds
#set(CMAKE_CXX_FLAGS "-fPIC -std=c++11 ${CMAKE_CXX_FLAGS}")

OPTION(JSONCPP_WITH_TESTS "" OFF)
OPTION(JSONCPP_WITH_POST_BUILD_UNITTEST "" OFF)
OPTION(PHYSFS_BUILD_SHARED "" OFF)
OPTION(PHYSFS_BUILD_TEST "" OFF)
OPTION(BUILD_TESTING "" OFF)
OPTION(BUILD_TESTS "" OFF)
OPTION(GLFW_INSTALL "" OFF)
#libs that we want to build
add_subdirectory("${BUILD_LIBRARIES_SOURCE_DIR}/freetype2/")
add_subdirectory("${BUILD_LIBRARIES_SOURCE_DIR}/glfw/")
add_subdirectory("${BUILD_LIBRARIES_SOURCE_DIR}/jsoncpp/")
add_subdirectory("${BUILD_LIBRARIES_SOURCE_DIR}/libccd/")
add_subdirectory("${BUILD_LIBRARIES_SOURCE_DIR}/physfs/")
add_subdirectory("${BUILD_LIBRARIES_SOURCE_DIR}/tinyxml2/")