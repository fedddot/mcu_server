add_executable(
	host_tools_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_api_request_reader.cpp
)

target_link_libraries(
	host_tools_tests
	PRIVATE
	api_request_reader
	ring_queue
	
	gtest
	gmock
)