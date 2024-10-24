add_executable(
	server_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ut_server.cpp
)

target_link_libraries(
	server_tests
	PRIVATE
    data_lib
    gpio
    gpio_manager
    request
    response
    server
    test_ipc_connection
    in_memory_inventory
    resources_vendor
    test_gpi
    test_gpo
    vendor
	
	gtest
)