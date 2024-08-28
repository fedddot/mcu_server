add_library(factory_fixture INTERFACE)
target_include_directories(factory_fixture INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
	factory_fixture
	INTERFACE
    gtest
    custom_creator
    data_lib
    default_gpio_tasks_data_retriever
    default_stepper_motor_tasks_data_retriever
    gpio
    gpio_tasks_factory
    gpio_test_data_creator
    inventory
    stepper_motor
    stepper_motor_tasks_factory
    stepper_motor_test_data_creator
    task_creator
    test_platform
)

add_executable(
	factory_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ut_factory.cpp
)

target_link_libraries(
	factory_tests
	PRIVATE
    factory_fixture
    factory
	gpio_tasks_factory
	stepper_motor_tasks_factory
	gtest
)