add_library(stepper_motor INTERFACE)
target_include_directories(stepper_motor INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(stepper_motor_manager INTERFACE)
target_include_directories(stepper_motor_manager INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    stepper_motor_manager
    INTERFACE
    data_lib
    gpio
    gpo
    gpo_stepper_motor
    inventory
    inventory_manager
    server_exception
    server_types
    stepper_motor
)

add_subdirectory(util)
add_subdirectory(tests EXCLUDE_FROM_ALL)