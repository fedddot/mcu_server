add_executable(
	ipc_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_http_ipc_server.cpp
)

target_link_libraries(
	ipc_tests
	PRIVATE
	http_ipc_server
	cpprest
	gtest
)