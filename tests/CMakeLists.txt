add_executable(
	e2e_mcu_control_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/e2e_mcu_control.cpp
)

target_link_libraries(
	e2e_mcu_control_tests
	PRIVATE

	gtest
	gmock
	-g
	-O0
)