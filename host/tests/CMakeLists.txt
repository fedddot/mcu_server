add_executable(
	host_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_host.cpp
)

target_link_libraries(
	host_tests
	PRIVATE
    host
    host_instance
    ipc_data_reader
    ipc_instance
    test_ipc_data_reader
    test_ipc_data_writer
    test_vendor
	gtest
)
