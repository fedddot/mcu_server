add_executable(
	movement_app_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_movement_host_builder.cpp
)

target_link_libraries(
	movement_app_tests
	PRIVATE
    axes_controller
    ipc_instance
    manager_instance
    movement_host_builder
    movement_manager_data
    test_ipc_data_reader
    test_ipc_data_writer
	
	gtest
)