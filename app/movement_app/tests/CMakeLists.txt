add_executable(
	movement_app_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_movement_host_builder.cpp
)

target_link_libraries(
	movement_app_tests
	PRIVATE
	test_ipc_data_reader
	test_ipc_data_writer
	movement_host_builder
	
	gtest
)