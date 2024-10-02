add_executable(
	ipc_util_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_buffered_custom_ipc_connection.cpp
)

target_link_libraries(
	ipc_util_tests
	PRIVATE
	buffered_custom_ipc_connection
	request
	response
	
	gtest
)
