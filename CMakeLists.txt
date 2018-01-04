cmake_minimum_required (VERSION 2.8.4)

if (CMAKE_VERSION VERSION_GREATER 3.1.3)
	cmake_policy(SET CMP0054 NEW)
endif ()

project (FCNPC)

# Definintions
set (INCLUDE_VERSION 180)
set (PLUGIN_VERSION "1.8.0")
add_definitions (-DPLUGIN_VERSION="${PLUGIN_VERSION}" -DINCLUDE_VERSION=${INCLUDE_VERSION})

set (CMAKE_SHARED_LIBRARY_PREFIX "")

set (LIBRARY_GROUP "lib\\")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	add_definitions (-DWIN32 -D_WIN32 -D__STDC__ -DSYSTEM_NAME="Windows")

	# *.def
	file (GLOB FCNPC_SRC
		${CMAKE_CURRENT_SOURCE_DIR}/src/Exports.def
	)

	# snprintf
	file (GLOB SNPRINTF_SRC
		${CMAKE_CURRENT_SOURCE_DIR}/lib/snprintf/*.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/lib/snprintf/*.h
	)
	set (PLATFORM_SRC ${SNPRINTF_SRC})
	source_group("${LIBRARY_GROUP}snprintf" FILES ${SNPRINTF_SRC})

	# flags
	set (CMAKE_C_FLAGS "/Zp1 /W4 /MT")
	set (CMAKE_CXX_FLAGS "/Zp1 /W4 /EHsc /MT")
	set (CMAKE_SHARED_LINKER_FLAGS "/NOLOGO /MACHINE:X86")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	add_definitions (-DLINUX -DSYSTEM_NAME="GNU/Linux")

	# fopen_s
	file (GLOB FOPEN_S_SRC
		${CMAKE_CURRENT_SOURCE_DIR}/lib/fopen_s/*.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/lib/fopen_s/*.h
	)
	set (PLATFORM_SRC ${FOPEN_S_SRC})
	source_group("${LIBRARY_GROUP}fopen_s" FILES ${FOPEN_S_SRC})

	# flags
	set (FLAGS "-m32 -fPIC -O2 -fno-stack-protector -fpack-struct=1 -Wall -Wno-attributes -Wno-strict-aliasing -Wno-sequence-point")
	set (CMAKE_C_FLAGS ${FLAGS})
	set (CMAKE_CXX_FLAGS "${FLAGS} -std=c++11")
	set (CMAKE_SHARED_LINKER_FLAGS "-fshort-wchar -shared")
endif ()

include_directories (
	${CMAKE_CURRENT_SOURCE_DIR}/src
	${CMAKE_CURRENT_SOURCE_DIR}/lib
	${CMAKE_CURRENT_SOURCE_DIR}/lib/sdk/amx
)

# subhook
set (SUBHOOK_STATIC true)
set (SUBHOOK_NO_TESTS true)
add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/lib/subhook
)

# FCNPC
file (GLOB FCNPC_SRC
	${FCNPC_SRC}
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp
)
source_group("" FILES ${FCNPC_SRC})

# MapAndreas 
file (GLOB MAPANDREAS_SRC 
	${CMAKE_CURRENT_SOURCE_DIR}/lib/mapandreas/*.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/lib/mapandreas/*.h 
) 
source_group("${LIBRARY_GROUP}MapAndreas" FILES ${MAPANDREAS_SRC}) 

# SDK
file (GLOB SDK_SRC
	${CMAKE_CURRENT_SOURCE_DIR}/lib/sdk/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/lib/sdk/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/lib/sdk/amx/*.c
	${CMAKE_CURRENT_SOURCE_DIR}/lib/sdk/amx/*.h
)
source_group("${LIBRARY_GROUP}SDK" FILES ${SDK_SRC})

# strlcpy
file (GLOB STRLCPY_SRC
	${CMAKE_CURRENT_SOURCE_DIR}/lib/strlcpy/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/lib/strlcpy/*.h
)
source_group("${LIBRARY_GROUP}strlcpy" FILES ${STRLCPY_SRC})

# RakNet
file (GLOB RAKNET_SRC
	${CMAKE_CURRENT_SOURCE_DIR}/lib/raknet/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/lib/raknet/*.h
)
source_group("${LIBRARY_GROUP}RakNet" FILES ${RAKNET_SRC})

# Library
add_library (${PROJECT_NAME} SHARED ${FCNPC_SRC}
                                    ${MAPANDREAS_SRC}
                                    ${SDK_SRC}
                                    ${STRLCPY_SRC}
                                    ${RAKNET_SRC}
                                    ${PLATFORM_SRC})

target_link_libraries (${PROJECT_NAME}
	subhook
	)
configure_file(src/FCNPC.inc.in ${CMAKE_CURRENT_SOURCE_DIR}/sampsvr_files/FCNPC.inc)
