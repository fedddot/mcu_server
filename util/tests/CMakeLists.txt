add_executable(
	mcu_server_util_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_json_data_parser.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_json_data_serializer.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_custom_receiver.cpp
)

target_link_libraries(
	mcu_server_util_tests
	PRIVATE
	json_data_parser
	json_data_serializer
	custom_receiver
	custom_listener
	listener

	gtest
)
