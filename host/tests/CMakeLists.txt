add_executable(
	host_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_host.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_host_sized_packages.cpp
)

target_link_libraries(
	host_tests
	PRIVATE
    host
    test_manager
	test_request_reader
	test_response_writer
	sized_package_reader
	sized_package_reader
	sized_package_infra

	gtest
	cpprest
)
