add_executable(
	protobuf_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ut_pb_request_reader.cpp
)

target_link_libraries(
	protobuf_tests
	PRIVATE
    pb_request_reader
    pb_response_writer
	gtest
)