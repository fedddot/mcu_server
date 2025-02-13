add_executable(
	protobuf_ipc_server_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_protobuf_ipc_server.cpp
)

target_link_libraries(
	protobuf_ipc_server_tests
	PRIVATE
	protobuf_ipc_server
	gtest
)