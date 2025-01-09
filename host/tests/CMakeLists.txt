
add_library(test_ipc_server INTERFACE)
target_include_directories(test_ipc_server INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(test_ipc_server INTERFACE ipc_server)

add_executable(
	host_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_host.cpp
)

target_link_libraries(
	host_tests
	PRIVATE
    host
    manager
    test_ipc_server
	gtest
)
