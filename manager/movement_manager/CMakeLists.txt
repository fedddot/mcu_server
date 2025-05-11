add_library(linear_movement INTERFACE)
target_include_directories(linear_movement INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(linear_movement INTERFACE movement_manager_data movement_manager_vector)

add_library(axes_controller INTERFACE)
target_include_directories(axes_controller INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(axes_controller INTERFACE movement_manager_data)

add_library(movement_manager INTERFACE)
target_include_directories(movement_manager INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    movement_manager
    INTERFACE
    manager
    manager_clonable
    movement_manager_request
    movement_manager_response
    linear_movement
)

add_subdirectory(data)
add_subdirectory(tests EXCLUDE_FROM_ALL)