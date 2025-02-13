add_library(test_ipc_server INTERFACE)
target_include_directories(test_ipc_server INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(test_ipc_server INTERFACE ipc_server)

add_executable(
	ipc_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_custom_ipc_server.cpp
)

target_link_libraries(
	ipc_tests
	PRIVATE
	custom_ipc_server
	gtest
)