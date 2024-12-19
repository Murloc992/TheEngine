cmake_minimum_required (VERSION 3.29)

project (ENGINE VERSION 1.0)

MESSAGE(STATUS "CMAKE_CXX_COMPILER_VERSION: " "${CMAKE_CXX_COMPILER_VERSION}")

set(CPP_GCC_COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++20 -march=haswell -ffast-math")
set(CPP_NMAKE_COMPILE_FLAGS 
"${CMAKE_CXX_FLAGS}"
"/std:c++20"	# the C++ standard to be uysed
"/FC"			# Display full compiled file path
#"/GL"			# Whole program optimization
"/GS-"			# Disables buffer security
#"/Gy"			# Enables function-level linking
#"/MD"			# MTDLL (MSVCRT.lib)
"/MP"			# multicore compilation
#"/O2"			# optimizations (favor speed), O2, Ox
#"/Ob2"			# force inline anything it can, Ob1 Ob2 Ob3
#"/Oi"			# generate intrinsic functions where applicable
#"/Ot"			# favor fast code
#"/Zi"			# generate debug information
#"/Zc:inline"	# remove unreferenced functions or data
#"/Zc:forScope"	# enforce `for` scoping rules
#"/sdl-"			# disable security features and warnings
#"/fp:fast"		# fast floating point
"/arch:AVX2"	# we support AVX2
# "-DNDEBUG"		# Not Debug
)

# Due to CMAKE using semicolon separation for lists, replace it with spaces instead
string (REPLACE ";" " " CPP_NMAKE_COMPILE_FLAGS "${CPP_NMAKE_COMPILE_FLAGS}")

set(ENGINE_SRC_PATH "src" )
set(LIB_PATH "libs" )
set(LIBBIN_PATH "${CMAKE_CURRENT_BINARY_DIR}/libs" )

#Includes
include_directories("${ENGINE_SRC_PATH}/"
	SYSTEM PUBLIC
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
if(WIN32 AND MSVC)
	add_definitions(/w)
else()
	add_definitions(-w)
endif()
add_subdirectory(libs/glad)
add_subdirectory(libs/libsigcplusplus)
if(WIN32 AND MSVC)
	remove_definitions(/w)
else()
	remove_definitions(-w)
endif()

add_library(engine STATIC
	${ENGINE_SOURCES}
	${ENGINE_INCLUDES}
)

#Only supporting gcc on linux, mingw(gcc)/nmake on windows
if(WIN32 AND MSVC)
	foreach(_source IN ITEMS ${ENGINE_SOURCES} ${ENGINE_INCLUDES})
		get_filename_component(_source_path "${_source}" PATH)
		string(REPLACE "${CMAKE_SOURCE_DIR}" "" _group_path "${_source_path}")
		string(REPLACE "/" "\\" _group_path "${_group_path}")
		source_group("${_group_path}" FILES "${_source}")
	endforeach()
	set_target_properties(engine PROPERTIES COMPILE_FLAGS "${CPP_NMAKE_COMPILE_FLAGS}")
	message(STATUS "Build flags: ${CPP_NMAKE_COMPILE_FLAGS}")
	#add_definitions(-DMSVC_COMPILE)# -DCMAKE_UNITY_BUILD=ON)
else()
	set_target_properties(engine PROPERTIES COMPILE_FLAGS "${CPP_GCC_COMPILE_FLAGS}")
	message(STATUS "Build flags: ${CPP_GCC_COMPILE_FLAGS}")
endif()

target_precompile_headers(engine PRIVATE "${ENGINE_SRC_PATH}/Precomp.h")

target_link_libraries(engine glfw freetype tinyxml2 jsoncpp_lib)

add_dependencies(engine glad sigc-3.0)