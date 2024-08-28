add_library(parser INTERFACE)
target_include_directories(parser INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(serializer INTERFACE)
target_include_directories(serializer INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(transformer INTERFACE)
target_include_directories(transformer INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(task_creator INTERFACE)
target_include_directories(task_creator INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    task_creator
    INTERFACE
    data
    task
    creator
)

add_library(mcu_factory INTERFACE)
target_include_directories(mcu_factory INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    mcu_factory
    INTERFACE
    data
    task
    creator
    task_creator
)

add_subdirectory(gpio_tasks_factory)
add_subdirectory(stepper_motor_tasks_factory)