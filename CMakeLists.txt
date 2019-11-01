cmake_minimum_required (VERSION 2.8)
project (ENGINE)

MESSAGE(STATUS "CMAKE_CXX_COMPILER_VERSION: " "${CMAKE_CXX_COMPILER_VERSION}")

set(CPP_GCC_COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -ggdb -w -std=c++17 -include Precomp.h")
set(CPP_NMAKE_COMPILE_FLAGS  "${CMAKE_CXX_FLAGS} /Gm- /std:c++latest /MP /O2 /W3")

#Only supporting gcc on linux, mingw(gcc)/nmake on windows
if(WIN32 AND NOT MINGW)
		set(CMAKE_CXX_FLAGS "${CPP_NMAKE_COMPILE_FLAGS}")
else()
	set(CMAKE_CXX_FLAGS "${CPP_GCC_COMPILE_FLAGS}")
endif()

set(ENGINE_SRC_PATH "src" )
set(LIB_PATH "libs" )
set(LIBBIN_PATH "build/libs" )

#define output library path for solutions
foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${LIB_PATH} )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${LIB_PATH} )
    set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${LIB_PATH} )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )

#Includes
include_directories("${ENGINE_SRC_PATH}/"
	"${LIB_PATH}"
	"${LIB_PATH}/freetype2/include"
	"${LIBBIN_PATH}/glad/include"
	"${LIB_PATH}/glfw/include"
	"${LIB_PATH}/glm/glm"
	"${LIB_PATH}/jsoncpp/include"
	"${LIB_PATH}/libccd/src"
	"${LIB_PATH}/libsigcplusplus"
	"${LIBBIN_PATH}/libsigcplusplus"
	"${LIB_PATH}/physfs/src"
	"${LIB_PATH}/stb"
	"${LIB_PATH}/tinyxml2"
)

file(GLOB_RECURSE ENGINE_INCLUDES
    "${ENGINE_SRC_PATH}/*.h"
    "${ENGINE_SRC_PATH}/*.hpp"
	"${ENGINE_SRC_PATH}/*.inl"
)

#Cross platform sources
file(GLOB_RECURSE ENGINE_SOURCES
	"${ENGINE_SRC_PATH}/*.c"
    "${ENGINE_SRC_PATH}/*.cpp"
		)

add_subdirectory(libs/glad)
add_subdirectory(libs/libsigcplusplus)

add_library(engine STATIC
	${ENGINE_SOURCES}
	${ENGINE_INCLUDES}
)