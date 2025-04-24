add_executable(
	movement_manager_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ut_movement_manager.cpp
)

target_link_libraries(
	movement_manager_tests
	PRIVATE
    movement_manager
    movement_manager_request
    movement_manager_response
    gtest
)