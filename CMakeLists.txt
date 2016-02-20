cmake_minimum_required (VERSION 2.8.4)

project (FCNPC)

set (VERSION "1.0.0")
add_definitions (-DVERSION="${VERSION}")

set (CMAKE_SHARED_LIBRARY_PREFIX "")

if (WIN32)
	add_definitions (-DWIN32 -D_WIN32 -D__STDC__)

	set (FCNPC_SRC
		${CMAKE_CURRENT_SOURCE_DIR}/src/Exports.def
		${CMAKE_CURRENT_SOURCE_DIR}/src/snprintf/*.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/snprintf/*.h
	)

	set (CMAKE_C_FLAGS "/Zp1")
	set (CMAKE_CXX_FLAGS "/Zp1 /EHsc")
	set (CMAKE_SHARED_LINKER_FLAGS "/NOLOGO /MACHINE:X86")
else ()
	add_definitions (-DLINUX)

	set (FCNPC_SRC
		${CMAKE_CURRENT_SOURCE_DIR}/src/fopen_s/*.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/fopen_s/*.h
	)

	set (CMAKE_C_FLAGS "-m32 -fPIC -O2 -w -fno-stack-protector -fpack-struct=1")
	set (CMAKE_CXX_FLAGS "-m32 -fPIC -O2 -w -fno-stack-protector -fpack-struct=1")
	set (CMAKE_SHARED_LINKER_FLAGS "-fshort-wchar -shared")
endif (WIN32)

include_directories (
	${CMAKE_CURRENT_SOURCE_DIR}/src
	${CMAKE_CURRENT_SOURCE_DIR}/src/sdk
	${CMAKE_CURRENT_SOURCE_DIR}/src/sdk/amx
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
	${CMAKE_CURRENT_SOURCE_DIR}/src/sdk/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/sdk/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/sdk/amx/*.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/sdk/amx/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/strlcpy/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/strlcpy/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/raknet/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/raknet/*.h
)

add_library (${PROJECT_NAME} SHARED ${FCNPC_SRC})

target_link_libraries (${PROJECT_NAME} subhook)
