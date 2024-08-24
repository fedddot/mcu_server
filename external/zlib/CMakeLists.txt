FetchContent_Declare(
    zlib
    URL  https://www.zlib.net/zlib-1.3.1.tar.gz
)
FetchContent_GetProperties(zlib)
if(NOT zlib_POPULATED)
	FetchContent_Populate(zlib)
	add_subdirectory(${zlib_SOURCE_DIR} ${zlib_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()