add_executable(
	stepper_motor_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ut_stepper_motor.cpp
)

target_link_libraries(
	stepper_motor_tests
	PRIVATE
    custom_creator
    gpio
    gpo
    stepper_motor
    test_gpo
    test_platform

	gtest
)