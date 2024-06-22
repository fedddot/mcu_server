add_library(gpio INTERFACE)
target_include_directories(gpio INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(gpi INTERFACE)
target_include_directories(gpi INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(gpi INTERFACE gpio)

add_library(gpo INTERFACE)
target_include_directories(gpo INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(gpo INTERFACE gpio)

add_library(inventory INTERFACE)
target_include_directories(inventory INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(inventory_task INTERFACE)
target_include_directories(inventory_task INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(inventory_task INTERFACE inventory task)

add_library(create_gpio_task INTERFACE)
target_include_directories(create_gpio_task INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    create_gpio_task
    INTERFACE
    creator
    data_lib
    gpio
    inventory
    inventory_task
    parser
)

add_library(delete_gpio_task INTERFACE)
target_include_directories(delete_gpio_task INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    delete_gpio_task
    INTERFACE
    data_lib
    gpio
    inventory
    inventory_task
    parser
)

add_library(get_gpio_task INTERFACE)
target_include_directories(get_gpio_task INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    get_gpio_task
    INTERFACE
    data_lib
    gpio
    gpi
    gpo
    inventory
    inventory_task
    parser
)

add_library(set_gpio_task INTERFACE)
target_include_directories(set_gpio_task INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    set_gpio_task
    INTERFACE
    data_lib
    gpio
    gpo
    inventory
    inventory_task
    parser
)

add_library(factory INTERFACE)
target_include_directories(factory INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(factory INTERFACE creator task parser)



add_library(server_task_engine INTERFACE)
target_include_directories(server_task_engine INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(server_task_engine INTERFACE engine)

add_subdirectory(tests EXCLUDE_FROM_ALL)