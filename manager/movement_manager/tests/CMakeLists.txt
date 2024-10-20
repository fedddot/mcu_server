add_library(test_movement INTERFACE)
target_include_directories(test_movement INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(test_movement INTERFACE data_lib movement)

add_executable(
	movement_manager_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ut_movement_manager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ut_linear_movement.cpp
)

target_link_libraries(
    movement_manager_tests
    PRIVATE
    data_lib
    in_memory_inventory
    movement
    movement_manager
    server_types
    test_movement
    gtest
)