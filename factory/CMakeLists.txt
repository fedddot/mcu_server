add_library(mcu_factory INTERFACE)
target_include_directories(mcu_factory INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    mcu_factory
    INTERFACE
    data_lib
    creator
    custom_creator
    gpio
    delay
    parser
    platform
    task
    
    create_gpio_task
    create_persistent_task
    delete_gpio_task
    delete_persistent_task
    set_gpio_task
    execute_persistent_task
    get_gpio_task
    sequence_task
    delay_task
)

add_subdirectory(tasks)
add_subdirectory(tests EXCLUDE_FROM_ALL)