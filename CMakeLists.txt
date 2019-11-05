cmake_minimum_required (VERSION 3.0)
project (ENGINE VERSION 1.0)

MESSAGE(STATUS "CMAKE_CXX_COMPILER_VERSION: " "${CMAKE_CXX_COMPILER_VERSION}")

set(CPP_GCC_COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -ggdb -w -std=c++17 -include Precomp.h")
set(CPP_NMAKE_COMPILE_FLAGS  "${CMAKE_CXX_FLAGS} /Gm- /std:c++latest /MP /W3")

set(ENGINE_SRC_PATH "src" )
set(LIB_PATH "libs" )
set(LIBBIN_PATH "${CMAKE_CURRENT_BINARY_DIR}/libs" )

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

#Generates files
add_subdirectory(libs/glad)
add_subdirectory(libs/libsigcplusplus)

add_library(engine STATIC
	${ENGINE_SOURCES}
	${ENGINE_INCLUDES}
)

#Only supporting gcc on linux, mingw(gcc)/nmake on windows
if(WIN32 AND NOT MINGW)
	set_target_properties(engine PROPERTIES COMPILE_FLAGS "${CPP_NMAKE_COMPILE_FLAGS}")
else()
	set_target_properties(engine PROPERTIES COMPILE_FLAGS "${CPP_GCC_COMPILE_FLAGS}")
endif()

target_precompile_headers(engine PRIVATE "${ENGINE_SRC_PATH}/Precomp.h")

target_link_libraries(engine)

add_dependencies(engine glad sigc-3.0)