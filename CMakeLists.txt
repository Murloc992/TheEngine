cmake_minimum_required (VERSION 3.0)
project (ENGINE VERSION 1.0)

MESSAGE(STATUS "CMAKE_CXX_COMPILER_VERSION: " "${CMAKE_CXX_COMPILER_VERSION}")

set(CPP_GCC_COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(CPP_NMAKE_COMPILE_FLAGS  "${CMAKE_CXX_FLAGS} /Gm- /std:c++latest /MP /W3 /sdl-")

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
	foreach(_source IN ITEMS ${ENGINE_SOURCES} ${ENGINE_INCLUDES})
		get_filename_component(_source_path "${_source}" PATH)
		string(REPLACE "${CMAKE_SOURCE_DIR}" "" _group_path "${_source_path}")
		string(REPLACE "/" "\\" _group_path "${_group_path}")
		source_group("${_group_path}" FILES "${_source}")
	endforeach()
	set_target_properties(engine PROPERTIES COMPILE_FLAGS "${CPP_NMAKE_COMPILE_FLAGS}")
	add_definitions(-DMSVC_COMPILE)# -DCMAKE_UNITY_BUILD=ON)
else()
	set_target_properties(engine PROPERTIES COMPILE_FLAGS "${CPP_GCC_COMPILE_FLAGS}")
endif()

target_precompile_headers(engine PRIVATE "${ENGINE_SRC_PATH}/Precomp.h")

target_link_libraries(engine glfw freetype tinyxml2 jsoncpp_lib)

add_dependencies(engine glad sigc-3.0)