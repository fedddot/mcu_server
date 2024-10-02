add_library(stepper_motor_manager INTERFACE)
target_include_directories(stepper_motor_manager INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    stepper_motor_manager
    INTERFACE
    stepper_motor
    inventory_manager
    resource
    response
    data_lib
)

add_subdirectory(tests EXCLUDE_FROM_ALL)