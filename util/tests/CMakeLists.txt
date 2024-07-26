add_executable(
	mcu_server_util_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_buffered_message_receiver.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_json_data_parser.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_json_data_serializer.cpp
)

target_link_libraries(
	mcu_server_util_tests
	PRIVATE
	buffered_message_receiver
	json_data_parser
	json_data_serializer
	data_lib
	custom_task
	
	gtest
)
