add_executable(
	host_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_host.cpp
)

target_link_libraries(
	host_tests
	PRIVATE
    host
    test_service
	test_ipc_data_reader
	test_ipc_data_writer

    gtest
)
