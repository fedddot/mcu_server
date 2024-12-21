add_executable(
	cnc_server_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ut_cnc_server.cpp
    # ${CMAKE_CURRENT_LIST_DIR}/src/ut_test_http_cnc_server.cpp
)

target_link_libraries(
	cnc_server_tests
	PRIVATE
    cnc_server
    custom_data_reader
    custom_data_writer
    data_lib
    gpio
    request
    response
    server_types
	
	gtest
)