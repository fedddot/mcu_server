add_executable(
	movement_app_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_movement_ipc_data_reader.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_movement_ipc_data_writer.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_request_json_transformer.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_movement_host.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_response_json_transformer.cpp
)

target_link_libraries(
	movement_app_tests
	PRIVATE
	movement_ipc_data_reader
	test_ipc_data_reader
	response_json_transformer
	
	movement_ipc_data_writer
	test_ipc_data_writer

	request_json_transformer
	movement_host

	test_axes_controller
	
	gtest
)