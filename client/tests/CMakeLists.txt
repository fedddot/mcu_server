add_executable(
	client_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_client.cpp
)

target_link_libraries(
	client_tests
	PRIVATE
    client
	server
	data_lib

	functional_connection

	json_data_parser
	json_data_serializer

	gtest
	gmock
	-g
	-O0
)