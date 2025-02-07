add_executable(
	stepper_motor_app_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_stepper_protobuf_ipc_server.cpp
)

target_link_libraries(
	stepper_motor_app_tests
	PRIVATE
	stepper_protobuf_ipc_server

	gtest
)