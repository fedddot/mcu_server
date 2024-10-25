add_executable(
	resources_vendor_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ut_resources_vendor.cpp
)

target_link_libraries(
	resources_vendor_tests
	PRIVATE
    data_lib
    in_memory_inventory
    inventory_manager
    manager
    request
    resources_vendor
    response
    server_types
    clonable_manager_wrapper
    
	gtest
)