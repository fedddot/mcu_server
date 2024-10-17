add_library(test_resource INTERFACE)
target_include_directories(test_resource INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(test_resource INTERFACE request resource response)

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
    test_resource
    test_gpi
    test_gpo
    vendor
	
	gtest
)