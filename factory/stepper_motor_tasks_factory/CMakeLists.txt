add_library(stepper_motor_tasks_data_retriever INTERFACE)
target_include_directories(stepper_motor_tasks_data_retriever INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(stepper_motor_tasks_data_retriever INTERFACE data stepper_motor)

add_library(default_stepper_motor_tasks_data_retriever INTERFACE)
target_include_directories(default_stepper_motor_tasks_data_retriever INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    default_stepper_motor_tasks_data_retriever
    INTERFACE
    data_lib
    stepper_motor
    stepper_motor_tasks_data_retriever
    stepper_motor_tasks_factory
)

add_library(stepper_motor_tasks_factory INTERFACE)
target_include_directories(stepper_motor_tasks_factory INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    stepper_motor_tasks_factory
    INTERFACE
    creator
    custom_task
    data
    inventory
    stepper_motor
    stepper_motor_tasks_data_retriever
    task
    task_creator
)

add_subdirectory(tests EXCLUDE_FROM_ALL)