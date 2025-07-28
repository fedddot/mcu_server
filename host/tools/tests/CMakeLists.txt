add_executable(
	host_tools_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_ring_data_buffer.cpp
)

target_link_libraries(
	host_tools_tests
	PRIVATE
	ring_data_buffer
	
	gtest
	gmock
)