
add_library(test_ipc_connection INTERFACE)
target_include_directories(test_ipc_connection INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(test_ipc_connection INTERFACE ipc_connection)

add_library(test_providers INTERFACE)
target_include_directories(test_providers INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(test_providers INTERFACE providers)

add_executable(
	host_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_host.cpp
)

target_link_libraries(
	host_tests
	PRIVATE
    host
	test_ipc_connection
    test_providers
	manager
	gtest
)
