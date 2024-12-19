add_executable(
	stepper_motor_manager_util_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ut_gpo_stepper_motor.cpp
)

target_link_libraries(
	stepper_motor_manager_util_tests
	PRIVATE
    gpo
    gpo_stepper_motor
    test_gpo

	gtest
)