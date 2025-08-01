add_executable(
	ipc_tools_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_ring_queue.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_package_reader.cpp
)

target_link_libraries(
	ipc_tools_tests
	PRIVATE
	ring_queue
	package_reader
	gtest
)