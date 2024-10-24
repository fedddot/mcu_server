add_executable(
	manager_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ut_inventory_manager.cpp
)

target_link_libraries(
	manager_tests
	PRIVATE
    data_lib
    in_memory_inventory
    inventory_manager
    server_types
	
	gtest
)