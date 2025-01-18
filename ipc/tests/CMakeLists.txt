add_library(test_ipc_server INTERFACE)
target_include_directories(test_ipc_server INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(test_ipc_server INTERFACE ipc_server)

add_executable(
	ipc_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_http_ipc_server.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_ipc_server_factory.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_buffered_ipc_server.cpp
)

target_link_libraries(
	ipc_tests
	PRIVATE
	ipc_server_factory
	buffered_ipc_server
	http_ipc_server
	
	cpprest
	gtest
)