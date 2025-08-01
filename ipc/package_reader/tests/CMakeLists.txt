add_executable(
	package_reader_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ut_package_reader.cpp
)

target_link_libraries(
	package_reader_tests
	PRIVATE
    package_reader
    package_utils
    ring_queue
	gtest
)