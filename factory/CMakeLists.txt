add_library(task_creator INTERFACE)
target_include_directories(task_creator INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    task_creator
    INTERFACE
    data
    task
    creator
)

add_library(factory INTERFACE)
target_include_directories(factory INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    factory
    INTERFACE
    data
    task
    creator
    task_creator
)

add_subdirectory(gpio_tasks_factory)
add_subdirectory(stepper_motor_tasks_factory)

add_subdirectory(tests EXCLUDE_FROM_ALL)