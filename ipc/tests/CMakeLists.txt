add_executable(
	ipc_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ut_buffered_package_reader.cpp
)

target_link_libraries(
	ipc_tests
	PRIVATE
    buffered_package_reader
    ipc_types
	
	gtest
)