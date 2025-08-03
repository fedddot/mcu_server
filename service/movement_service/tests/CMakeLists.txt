add_executable(
	movement_service_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ut_movement_service.cpp
)

target_link_libraries(
	movement_service_tests
	PRIVATE
    axes_controller
    axes_controller_config_request
    linear_movement_request
    movement_manager
    movement_manager_data
    movement_manager_vector
    movement_service
    movement_service_api_response
    test_axes_controller
	
    gtest
)