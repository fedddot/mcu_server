add_library(cnc_server INTERFACE)
target_include_directories(cnc_server INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    cnc_server
    INTERFACE
    clonable_manager_wrapper
    data_lib
    gpio
    gpio_manager
    in_memory_inventory
    linear_movement
    movement
    movement_manager
    server
    server_exception
    server_types
    stepper_motor
    stepper_motor_manager
    vendor
    vector
    circular_movement
)

add_subdirectory(tests EXCLUDE_FROM_ALL)