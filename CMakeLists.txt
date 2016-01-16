cmake_minimum_required (VERSION 2.8.4)

project (FCNPC)

set (VERSION "0.1 B15 R2")
add_definitions (-DVERSION="${VERSION}")

set (CMAKE_SHARED_LIBRARY_PREFIX "")

if (WIN32)
	add_definitions (-DWIN32 -D_WIN32 -D__STDC__)

	set (FCNPC_SRC
		${CMAKE_CURRENT_SOURCE_DIR}/src/Exports.def
		${CMAKE_CURRENT_SOURCE_DIR}/src/snprintf/*.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/snprintf/*.h
	)

	set (CMAKE_SHARED_LINKER_FLAGS "/NOLOGO /MACHINE:X86")
else ()
	add_definitions (-DLINUX)

	set (FCNPC_SRC
		${CMAKE_CURRENT_SOURCE_DIR}/src/fopen_s/*.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/fopen_s/*.h
	)

	set (CMAKE_C_FLAGS "-m32 -fPIC -O2 -w")
	set (CMAKE_CXX_FLAGS "-m32 -fPIC -O2 -w")
	set (CMAKE_SHARED_LINKER_FLAGS "-m32 -O2 -fshort-wchar -shared")
endif (WIN32)

include_directories (
	${CMAKE_CURRENT_SOURCE_DIR}/src
	${CMAKE_CURRENT_SOURCE_DIR}/src/SDK
	${CMAKE_CURRENT_SOURCE_DIR}/src/SDK/amx
)

set (SUBHOOK_STATIC true)
set (SUBHOOK_NO_TESTS true)
add_subdirectory (
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
	${CMAKE_CURRENT_SOURCE_DIR}/src/strlcpy/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/strlcpy/*.h
)

add_library (${PROJECT_NAME} SHARED ${FCNPC_SRC})

target_link_libraries (${PROJECT_NAME} subhook)
