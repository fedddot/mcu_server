add_library(server_fixture INTERFACE)
target_include_directories(server_fixture INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
	server_fixture
	INTERFACE
    custom_creator
    data_lib
    default_gpio_tasks_data_retriever
    default_stepper_motor_tasks_data_retriever
    factory
    gpio
    gpio_tasks_factory
    inventory
    server
    stepper_motor
    stepper_motor_tasks_factory
    test_platform
    gtest
)

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
    gpio_test_data_creator
    server
    server_fixture
    stepper_motor_test_data_creator

	gtest
)