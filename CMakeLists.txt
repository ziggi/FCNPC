cmake_minimum_required (VERSION 2.8.11)

project (FCNPC)

set (VERSION "0.1 B15 R2")
add_definitions (-DVERSION="${VERSION}")

set (CMAKE_SHARED_LIBRARY_PREFIX "")

if (WIN32)
	add_definitions (-DWIN32 -D_WIN32 -D_CRT_SECURE_NO_WARNINGS -D__STDC__)

	file (GLOB FCNPC_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/subhook/subhook_windows.c)

	set (CMAKE_SHARED_LINKER_FLAGS "/NOLOGO /MACHINE:X86 /DEF:\"${CMAKE_CURRENT_SOURCE_DIR}/src/Exports.def\"")
else ()
	add_definitions (-DLINUX)

	file (GLOB FCNPC_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/subhook/subhook_linux.c)

	set (CMAKE_C_FLAGS "-m32 -fPIC -O2 -w")
	set (CMAKE_CXX_FLAGS "-m32 -fPIC -O2 -w")
	set (CMAKE_SHARED_LINKER_FLAGS "-m32 -O2 -fshort-wchar -shared")
endif (WIN32)

include_directories (
	${CMAKE_CURRENT_SOURCE_DIR}/src
	${CMAKE_CURRENT_SOURCE_DIR}/src/SDK
	${CMAKE_CURRENT_SOURCE_DIR}/src/SDK/amx
	${CMAKE_CURRENT_SOURCE_DIR}/src/subhook
)

file (GLOB FCNPC_SRC
	${FCNPC_SRC}
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/SDK/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/SDK/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/SDK/amx/*.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/SDK/amx/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/subhook/subhook.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/subhook/subhook_x86.c
)

add_library (FCNPC SHARED ${FCNPC_SRC})
