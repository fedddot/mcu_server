add_library(gpio_tasks_data_retriever INTERFACE)
target_include_directories(gpio_tasks_data_retriever INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(gpio_tasks_data_retriever INTERFACE data gpio)

add_library(default_gpio_tasks_data_retriever INTERFACE)
target_include_directories(default_gpio_tasks_data_retriever INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(default_gpio_tasks_data_retriever INTERFACE gpio_tasks_data_retriever data_lib gpio)

add_library(gpio_tasks_factory INTERFACE)
target_include_directories(gpio_tasks_factory INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    gpio_tasks_factory
    INTERFACE
    custom_task
    data
    gpio
    gpio_tasks_data_retriever
    inventory
    platform
    task
    task_creator
)

add_subdirectory(tests EXCLUDE_FROM_ALL)