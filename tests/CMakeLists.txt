add_executable(
	e2e_mcu_control_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/e2e_mcu_control.cpp
)

target_link_libraries(
	e2e_mcu_control_tests
	PRIVATE
    client
    connection
    data_lib
    functional_parser
    functional_serializer
    json_data_parser
    json_data_serializer
    server
    server_task_engine
    test_gpi
    test_gpo

	gtest
	gmock
)