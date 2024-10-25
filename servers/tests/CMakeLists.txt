add_executable(
	cnc_server_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ut_cnc_server.cpp
)

target_link_libraries(
	cnc_server_tests
	PRIVATE
    clonable_manager_wrapper
    data_lib
    gpio
    gpio_manager
    request
    response
    cnc_server
    server_types
    test_ipc_connection
    in_memory_inventory
    resources_vendor
    test_gpi
    test_gpo
    vendor
	
	gtest
)