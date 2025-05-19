add_executable(
	movement_vendor_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ut_movement_vendor.cpp
)

target_link_libraries(
	movement_vendor_tests
	PRIVATE
    axes_controller
    linear_movement_request
    movement_manager
    movement_manager_data
    movement_manager_vector
    movement_vendor
    movement_vendor_api_response
    test_axes_controller
	
    gtest
)