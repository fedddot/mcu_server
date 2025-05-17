add_executable(
	movement_vendor_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ut_movement_vendor.cpp
)

target_link_libraries(
	movement_vendor_tests
	PRIVATE
    custom_manager
    linear_movement_request
    manager
    movement_manager_request
    movement_manager_response
    movement_manager_vector
    movement_vendor
    movement_vendor_api_request
    vendor_instance
	
    gtest
)