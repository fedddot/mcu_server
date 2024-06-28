add_executable(
	client_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_client.cpp
)

target_link_libraries(
	client_tests
	PRIVATE
    client
	data_lib
	functional_connection

	gtest
	gmock
)