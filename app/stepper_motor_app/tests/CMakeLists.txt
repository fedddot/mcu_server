add_executable(
	stepper_motor_app_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_stepper_ipc_data_reader.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_stepper_ipc_data_writer.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_stepper_host.cpp
)

target_link_libraries(
	stepper_motor_app_tests
	PRIVATE
	stepper_ipc_data_reader
	test_ipc_data_reader
	
	stepper_ipc_data_writer
	test_ipc_data_writer

	stepper_ipc_data_infra

	stepper_host
	test_stepper_motor
	stepper_motor_data
	stepper_motor_request
	stepper_motor_response

	gtest
)