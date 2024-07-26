add_executable(
	mcu_server_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ut_mcu_server.cpp
)

target_link_libraries(
	mcu_server_tests
	PRIVATE
	mcu_server
	buffered_message_receiver
	json_data_parser
	json_data_serializer
	custom_sender

	gtest
)