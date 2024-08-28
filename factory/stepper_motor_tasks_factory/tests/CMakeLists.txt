add_library(stepper_motor_tasks_factory_fixture INTERFACE)
target_include_directories(stepper_motor_tasks_factory_fixture INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
	stepper_motor_tasks_factory_fixture
	INTERFACE
    custom_creator
    data_lib
    gpio
    stepper_motor_tasks_factory
    default_stepper_motor_tasks_data_retriever
    test_platform
)

add_executable(
	stepper_motor_tasks_factory_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ut_stepper_motor_tasks_factory.cpp
)

target_link_libraries(
	stepper_motor_tasks_factory_tests
	PRIVATE
    stepper_motor_tasks_factory_fixture
	gtest
)