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
    test_manager
	test_ipc_data_reader
	test_ipc_data_writer
	raw_data_package_reader
	raw_data_package_reader
	raw_data_package_infra

	gtest
	cpprest
)
