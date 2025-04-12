add_executable(
	json_data_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ut_json_ipc_data_reader.cpp
	# ${CMAKE_CURRENT_LIST_DIR}/src/ut_json_ipc_data_writer.cpp
)

target_link_libraries(
	json_data_tests
	PRIVATE
    json_ipc_data_reader
	json_ipc_data_writer

	jsoncpp_static
	gtest
)