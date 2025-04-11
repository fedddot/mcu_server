add_executable(
	json_package_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ut_json_request_reader.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_json_response_writer.cpp
)

target_link_libraries(
	json_package_tests
	PRIVATE
    json_request_reader
	json_response_writer
	gtest
)