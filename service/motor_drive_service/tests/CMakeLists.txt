add_executable(
	motor_drive_service_tests
	${CMAKE_CURRENT_LIST_DIR}/src/ut_motor_drive_service.cpp
)

target_link_libraries(
	motor_drive_service_tests
	PRIVATE
    motor_drive_service
    gtest
	gmock
	gtest_main
)

add_test(NAME motor_drive_service_tests COMMAND motor_drive_service_tests)