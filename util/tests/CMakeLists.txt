add_executable(
	server_util_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_json_data_parser.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_json_request_parser.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_json_data_serializer.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_http_ipc_connection.cpp
)

target_link_libraries(
	server_util_tests
	PRIVATE
	json_data_parser
	json_request_parser
	json_data_serializer
	data_lib
	server_types
	http_ipc_connection
	
	gtest
)
