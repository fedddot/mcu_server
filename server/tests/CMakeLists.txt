add_executable(
	server_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_server.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_create_gpio_task.cpp
)

target_link_libraries(
	server_tests
	PRIVATE
    server

    data_lib

    functional_connection
    functional_creator
    functional_parser
	json_data_parser
    functional_serializer
	json_data_serializer
    functional_task
	test_gpo
	test_gpi

	create_gpio_task
	gtest
	gmock
	-g
	-O0
)