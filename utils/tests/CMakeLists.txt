add_executable(
	mcu_control_utils_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_serial_connection.cpp
)

target_link_libraries(
	mcu_control_utils_tests
	PRIVATE
    serial_connection

	gtest
	gmock
)