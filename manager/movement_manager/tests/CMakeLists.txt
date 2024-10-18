add_executable(
	movement_manager_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ut_movement_manager.cpp
)

target_link_libraries(
	movement_manager_tests
	PRIVATE
    stepper_motor
    movement_manager
    in_memory_inventory
    server_types
    test_stepper_motor
	gtest
)