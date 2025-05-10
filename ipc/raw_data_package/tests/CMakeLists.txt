add_executable(
	raw_data_package_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ut_raw_data_package_reader.cpp
	# ${CMAKE_CURRENT_LIST_DIR}/src/ut_raw_data_package_writer.cpp
)

target_link_libraries(
	raw_data_package_tests
	PRIVATE
	ipc_data
    raw_data_package_reader
	# raw_data_package_writer
	raw_data_package_descriptor
	raw_data_package_utils
	gtest
)