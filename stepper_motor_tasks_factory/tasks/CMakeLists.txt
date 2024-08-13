add_library(create_stepper_motor_task INTERFACE)
target_include_directories(create_stepper_motor_task INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    create_stepper_motor_task
    INTERFACE
    creator
    data
    delay
    gpo
    inventory
    stepper_motor
    task
)

add_library(delete_stepper_motor_task INTERFACE)
target_include_directories(delete_stepper_motor_task INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    delete_stepper_motor_task
    INTERFACE
    creator
    data
    inventory
    stepper_motor
    task
)

add_library(steps_task INTERFACE)
target_include_directories(steps_task INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    steps_task
    INTERFACE
    creator
    data
    inventory
    task
)