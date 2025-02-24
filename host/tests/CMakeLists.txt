add_executable(
	host_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_host.cpp
)

target_link_libraries(
	host_tests
	PRIVATE
    host
    test_manager
	test_request_reader
	test_response_writer
	
	gtest
	cpprest
)
