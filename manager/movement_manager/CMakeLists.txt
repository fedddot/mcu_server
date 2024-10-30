add_library(movement INTERFACE)
target_include_directories(movement INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(movement INTERFACE data_lib)

add_library(linear_movement INTERFACE)
target_include_directories(linear_movement INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    linear_movement
    INTERFACE
    data_lib
    inventory
    movement
    server_exception
    server_types
    stepper_motor
)

add_library(movement_manager INTERFACE)
target_include_directories(movement_manager INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    movement_manager
    INTERFACE
    data_lib
    movement
    inventory
    inventory_manager
    server_types
)

add_library(vector INTERFACE)
target_include_directories(movement_manager INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(movement_model INTERFACE)
target_include_directories(movement_model INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(movement_model INTERFACE vector)

add_library(circular_movement_model INTERFACE)
target_include_directories(circular_movement_model INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(circular_movement_model INTERFACE movement_model vector)

add_subdirectory(tests EXCLUDE_FROM_ALL)