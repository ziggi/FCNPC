cmake_minimum_required (VERSION 2.8.11)

project (FCNPC)

set (VERSION "0.1 B15 R2")
add_definitions (-DVERSION="${VERSION}")

if (WIN32)
	add_definitions(-DWIN32 -D_WIN32)

	set(CMAKE_C_FLAGS "/nologo /EHsc /MT")
	set(CMAKE_CXX_FLAGS "/nologo /EHsc /MT")
	set(CMAKE_SHARED_LINKER_FLAGS "/NOLOGO /MACHINE:X86 /DEF:\"${CMAKE_CURRENT_SOURCE_DIR}/src/Exports.def\"")

	set(CMAKE_C_FLAGS_DEBUG "/Od /Zi")
	set(CMAKE_CXX_FLAGS_DEBUG "/Od /Zi")
	set(CMAKE_C_FLAGS_RELEASE "/Ox")
	set(CMAKE_CXX_FLAGS_RELEASE "/Ox")
	set(CMAKE_C_FLAGS_RELWITHDEBINFO "/Ox /Zi")
	set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/Ox /Zi")
else ()
	add_definitions(-DLINUX)

	set(CMAKE_C_FLAGS "-m32")
	set(CMAKE_CXX_FLAGS "-m32 -std=c++11 -Wno-write-strings")
	set(CMAKE_SHARED_LINKER_FLAGS "-m32 -fshort-wchar -shared")

	set(CMAKE_C_FLAGS_DEBUG "-O0 -g3")
	set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3")
	set(CMAKE_C_FLAGS_RELEASE "-O3 -g0")
	set(CMAKE_CXX_FLAGS_RELEASE "-O3 -g0")
	set(CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g3")
	set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g3")

	set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "-O3 -s")
endif(WIN32)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src" "${CMAKE_CURRENT_SOURCE_DIR}/src/SDK" "${CMAKE_CURRENT_SOURCE_DIR}/src/SDK/amx" "${CMAKE_CURRENT_SOURCE_DIR}/src/subhook")

set (SOURCE_LIB 
	"${CMAKE_CURRENT_SOURCE_DIR}/src"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/SDK"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/SDK"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/subhook"
)

add_library (FCNPC SHARED ${SOURCE_LIB})
