add_library(test_stepper_motor INTERFACE)
target_include_directories(test_stepper_motor INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_executable(
	stepper_motor_manager_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ut_stepper_motor_manager.cpp
)

target_link_libraries(
	stepper_motor_manager_tests
	PRIVATE
    test_stepper_motor
    data_lib
    stepper_motor
    stepper_motor_manager
    request
    response

	gtest
)