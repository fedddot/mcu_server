add_library(stepper_motor_tasks_factory INTERFACE)
target_include_directories(stepper_motor_tasks_factory INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    stepper_motor_tasks_factory
    INTERFACE
    create_stepper_motor_task
    delete_stepper_motor_task
    steps_task
    stepper_motor
    data_lib
    task
    creator
)

add_subdirectory(tasks)
add_subdirectory(tests EXCLUDE_FROM_ALL)