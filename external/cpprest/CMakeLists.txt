FetchContent_Declare(
    cpprest
    GIT_REPOSITORY  https://github.com/microsoft/cpprestsdk.git
    GIT_TAG         v2.10.19
)
FetchContent_GetProperties(cpprest)
if(NOT cpprest_POPULATED)
	FetchContent_Populate(cpprest)
	add_subdirectory(${cpprest_SOURCE_DIR} ${cpprest_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()