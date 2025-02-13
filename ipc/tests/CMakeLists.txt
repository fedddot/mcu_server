add_executable(
	custom_ipc_server_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_custom_ipc_server.cpp
)

target_link_libraries(
	custom_ipc_server_tests
	PRIVATE
	custom_ipc_server
	gtest
)