add_executable(
	host_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_host.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_host_raw_data_package.cpp
)

target_link_libraries(
	host_tests
	PRIVATE
    host
    json_ipc_data_reader
    json_ipc_data_writer
    raw_data_package_descriptor
    raw_data_package_reader
    raw_data_package_utils
    raw_data_package_writer
    shared_ipc_data_reader
    shared_ipc_data_writer
	test_ipc_data_reader
	test_ipc_data_writer
    test_manager

	jsoncpp_static
	gtest
	cpprest
)
