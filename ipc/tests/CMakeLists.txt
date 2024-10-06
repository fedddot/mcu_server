add_library(test_ipc_connection INTERFACE)
target_include_directories(test_ipc_connection INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(test_ipc_connection INTERFACE request ipc_connection response)

add_executable(
	ipc_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_buffered_ipc_connection.cpp
)

target_link_libraries(
	ipc_tests
	PRIVATE
	buffered_ipc_connection	
	gtest
)
