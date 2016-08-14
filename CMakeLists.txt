cmake_minimum_required (VERSION 2.8.4)

if (CMAKE_VERSION VERSION_GREATER 3.1.3)
	cmake_policy(SET CMP0054 NEW)
endif ()

project (FCNPC)

set (INCLUDE_VERSION 113)
set (PLUGIN_VERSION "1.1.3")
add_definitions (-DPLUGIN_VERSION="${PLUGIN_VERSION}" -DINCLUDE_VERSION=${INCLUDE_VERSION})

set (CMAKE_SHARED_LIBRARY_PREFIX "")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	add_definitions (-DWIN32 -D_WIN32 -D__STDC__ -DSYSTEM_NAME="Windows")

	set (FCNPC_SRC
		${CMAKE_CURRENT_SOURCE_DIR}/src/Exports.def
		${CMAKE_CURRENT_SOURCE_DIR}/lib/snprintf/*.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/lib/snprintf/*.h
	)

	set (CMAKE_C_FLAGS "/Zp1 /W4")
	set (CMAKE_CXX_FLAGS "/Zp1 /W4 /EHsc")
	set (CMAKE_SHARED_LINKER_FLAGS "/NOLOGO /MACHINE:X86")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	add_definitions (-DLINUX -DSYSTEM_NAME="GNU/Linux")

	set (FCNPC_SRC
		${CMAKE_CURRENT_SOURCE_DIR}/lib/fopen_s/*.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/lib/fopen_s/*.h
	)

	set (FLAGS "-m32 -fPIC -O2 -fno-stack-protector -fpack-struct=1 -Wall -Wno-attributes -Wno-strict-aliasing -Wno-sequence-point")
	set (CMAKE_C_FLAGS ${FLAGS})
	set (CMAKE_CXX_FLAGS "${FLAGS} -std=c++11")
	set (CMAKE_SHARED_LINKER_FLAGS "-fshort-wchar -shared")
endif ()

include_directories (
	${CMAKE_CURRENT_SOURCE_DIR}/src
	${CMAKE_CURRENT_SOURCE_DIR}/lib
)

set (SUBHOOK_STATIC true)
set (SUBHOOK_NO_TESTS true)
add_subdirectory (
	${CMAKE_CURRENT_SOURCE_DIR}/lib/subhook
)

file (GLOB FCNPC_SRC
	${FCNPC_SRC}
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/lib/samp-plugin-sdk/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/lib/samp-plugin-sdk/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/lib/samp-plugin-sdk/sdk/amx/*.c
	${CMAKE_CURRENT_SOURCE_DIR}/lib/samp-plugin-sdk/sdk/amx/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/lib/strlcpy/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/lib/strlcpy/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/lib/raknet/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/lib/raknet/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/lib/mapandreas/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/lib/mapandreas/*.h
)

add_library (${PROJECT_NAME} SHARED ${FCNPC_SRC})

target_link_libraries (${PROJECT_NAME} subhook)
configure_file(pawn/FCNPC.inc.in pawn/FCNPC.inc)
