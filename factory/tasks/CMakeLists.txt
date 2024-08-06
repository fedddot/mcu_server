add_library(create_gpio_task INTERFACE)
target_include_directories(create_gpio_task INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    create_gpio_task
    INTERFACE
    creator
    data
    gpio
    inventory
    task
)

add_library(create_persistent_task INTERFACE)
target_include_directories(create_persistent_task INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    create_persistent_task
    INTERFACE
    creator
    data
    inventory
    task
)

add_library(delete_persistent_task INTERFACE)
target_include_directories(delete_persistent_task INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    delete_persistent_task
    INTERFACE
    creator
    data
    inventory
    task
)

add_library(delete_gpio_task INTERFACE)
target_include_directories(delete_gpio_task INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    delete_gpio_task
    INTERFACE
    creator
    data
    gpio
    inventory
    task
)

add_library(get_gpio_task INTERFACE)
target_include_directories(get_gpio_task INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    get_gpio_task
    INTERFACE
    creator
    data
    gpio
    gpi
    gpo
    inventory
    task
)

add_library(set_gpio_task INTERFACE)
target_include_directories(set_gpio_task INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    set_gpio_task
    INTERFACE
    creator
    data
    gpio
    gpo
    inventory
    task
)

add_library(execute_persistent_task INTERFACE)
target_include_directories(execute_persistent_task INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    execute_persistent_task
    INTERFACE
    data
    inventory
    task
)

add_library(execute_persistent_tasks INTERFACE)
target_include_directories(execute_persistent_tasks INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    execute_persistent_tasks
    INTERFACE
    data_lib
    inventory
    task
)

add_library(sequence_task INTERFACE)
target_include_directories(sequence_task INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    sequence_task
    INTERFACE
    array
    creator
    data
    task
)

add_library(delay_task INTERFACE)
target_include_directories(delay_task INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    delay_task
    INTERFACE
    creator
    data
    delay
    task
)

add_subdirectory(tests EXCLUDE_FROM_ALL)