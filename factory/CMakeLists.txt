add_library(mcu_factory_parsers INTERFACE)
target_include_directories(mcu_factory_parsers INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    mcu_factory_parsers
    INTERFACE
    data_lib
    gpio
    parser
)

add_library(default_mcu_factory_parsers INTERFACE)
target_include_directories(default_mcu_factory_parsers INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    default_mcu_factory_parsers
    INTERFACE
    data_lib
    gpio
    parser
    custom_parser
)

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
    mcu_factory_parsers
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