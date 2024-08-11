add_executable(
	stepper_motor_tasks_factory_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ut_stepper_motor_tasks_factory.cpp
)

target_link_libraries(
	stepper_motor_tasks_factory_tests
	PRIVATE
    stepper_motor_tasks_factory

	gtest
)