add_executable(
	stepper_motor_manager_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ut_stepper_motor_manager.cpp
)

target_link_libraries(
	stepper_motor_manager_tests
	PRIVATE
    data_lib
    stepper_motor
    stepper_motor_manager
    request
    response

	gtest
)