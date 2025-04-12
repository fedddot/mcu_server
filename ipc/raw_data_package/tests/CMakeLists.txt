add_executable(
	sized_package_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ut_raw_data_package_reader.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_raw_data_package_writer.cpp
)

target_link_libraries(
	sized_package_tests
	PRIVATE
    raw_data_package_reader
	raw_data_package_writer
	sized_package_common
	gtest
)