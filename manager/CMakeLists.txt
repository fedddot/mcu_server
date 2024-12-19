add_library(inventory INTERFACE)
target_include_directories(inventory INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(inventory_manager INTERFACE)
target_include_directories(inventory_manager INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    inventory_manager
    INTERFACE
    data_lib
    inventory
    manager
    server_exception
    server_types
)

add_subdirectory(gpio_manager)
add_subdirectory(movement_manager)
add_subdirectory(stepper_motor_manager)
add_subdirectory(utils)
add_subdirectory(tests EXCLUDE_FROM_ALL)