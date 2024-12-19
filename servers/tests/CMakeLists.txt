add_executable(
	cnc_server_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ut_cnc_server.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ut_test_http_cnc_server.cpp
)

target_link_libraries(
	cnc_server_tests
	PRIVATE
    cnc_server
    data_lib
    gpio
    linear_movement
    test_http_cnc_server
    response
    server_types
    stepper_motor
    test_gpi
    test_gpo
    test_ipc_connection
	
	gtest
)