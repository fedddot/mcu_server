add_executable(
	inventory_manager_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ut_inventory_manager.cpp
)

target_link_libraries(
	inventory_manager_tests
	PRIVATE
    inventory_manager
	test_resource

	gtest
)