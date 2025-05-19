add_library(test_axes_controller INTERFACE)
target_include_directories(test_axes_controller INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(test_axes_controller INTERFACE movement_manager_data axes_controller)

add_executable(
	movement_manager_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    # ${CMAKE_CURRENT_LIST_DIR}/src/ut_linear_movement.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ut_movement_manager.cpp
)

target_link_libraries(
	movement_manager_tests
	PRIVATE
    linear_movement
    movement_manager
    test_axes_controller
    gtest
)