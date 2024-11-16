add_library(test_stepper_motor INTERFACE)
target_include_directories(test_stepper_motor INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(test_stepper_motor INTERFACE stepper_motor)

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
    in_memory_inventory
    server_types
    test_gpo

	gtest
)