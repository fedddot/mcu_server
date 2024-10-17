add_library(stepper_motor INTERFACE)
target_include_directories(stepper_motor INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(stepper_motor_manager INTERFACE)
target_include_directories(stepper_motor_manager INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    stepper_motor_manager
    INTERFACE
    data_lib
    inventory_manager
    resource
    stepper_motor
)

add_subdirectory(tests EXCLUDE_FROM_ALL)