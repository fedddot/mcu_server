add_executable(
	requests_manager_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ut_requests_manager.cpp
)

target_link_libraries(
	requests_manager_tests
	PRIVATE
    requests_manager
    request
    response
    in_memory_inventory
    data_lib
	gtest
)