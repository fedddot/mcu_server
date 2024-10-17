add_library(inventory INTERFACE)
target_include_directories(inventory INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(in_memory_inventory INTERFACE)
target_include_directories(in_memory_inventory INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    in_memory_inventory
    INTERFACE
    inventory
)

add_library(inventory_manager INTERFACE)
target_include_directories(inventory_manager INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    inventory_manager
    INTERFACE
    data_lib
    inventory
    request
    resource
    response
)

add_subdirectory(gpio_manager)
add_subdirectory(requests_manager)
add_subdirectory(stepper_motor_manager)

add_subdirectory(tests EXCLUDE_FROM_ALL)