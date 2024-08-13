add_library(stepper_motor_tasks_factory_fixture INTERFACE)
target_include_directories(stepper_motor_tasks_factory_fixture INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(stepper_motor_tasks_factory_fixture INTERFACE stepper_motor_tasks_factory)

add_executable(
	stepper_motor_tasks_factory_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ut_stepper_motor_tasks_factory.cpp
)

target_link_libraries(
	stepper_motor_tasks_factory_tests
	PRIVATE
    stepper_motor_tasks_factory
	custom_parser
	stepper_motor_tasks_factory_fixture
	test_gpo
	gtest
)