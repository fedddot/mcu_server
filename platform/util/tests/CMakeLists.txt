add_executable(
	mcu_platform_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_buffered_message_receiver.cpp
)

target_link_libraries(
	mcu_platform_tests
	PRIVATE
	buffered_message_receiver
	data_lib
	
	gtest
)
