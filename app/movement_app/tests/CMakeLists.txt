add_executable(
	movement_app_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_movement_ipc_data_reader.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_movement_ipc_data_writer.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_movement_host.cpp
)

target_link_libraries(
	movement_app_tests
	PRIVATE
	movement_ipc_data_reader
	test_ipc_data_reader
	
	movement_ipc_data_writer
	test_ipc_data_writer

	movement_ipc_data_infra

	movement_host
	test_movement
	movement_data
	movement_request
	movement_response

	gtest
)