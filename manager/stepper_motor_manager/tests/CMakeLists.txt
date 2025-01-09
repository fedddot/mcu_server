add_library(test_stepper_motor INTERFACE)
target_include_directories(test_stepper_motor INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(test_stepper_motor INTERFACE stepper_motor stepper_motor_types)

add_library(test_stepper_motor_creator INTERFACE)
target_include_directories(test_stepper_motor_creator INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(test_stepper_motor_creator INTERFACE stepper_motor stepper_motor_types test_stepper_motor)

add_library(test_stepper_motor_delay_generator INTERFACE)
target_include_directories(test_stepper_motor_delay_generator INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(test_stepper_motor_delay_generator INTERFACE stepper_motor_delay_generator)

add_executable(
	stepper_motor_manager_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ut_stepper_motor_manager.cpp
)

target_link_libraries(
	stepper_motor_manager_tests
	PRIVATE
    stepper_motor_manager
    stepper_motor_request
    stepper_motor_response
    stepper_motor_types
    test_stepper_motor_creator
    test_stepper_motor_delay_generator
	gtest
)