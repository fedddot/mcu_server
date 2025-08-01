add_executable(
	api_request_reader_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ut_api_request_reader.cpp
)

target_link_libraries(
	api_request_reader_tests
	PRIVATE
    api_request_reader
    package_utils
    ring_queue
	gtest
)