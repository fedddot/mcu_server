add_library(inventory INTERFACE)
target_include_directories(inventory INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(in_memory_inventory INTERFACE)
target_include_directories(in_memory_inventory INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    in_memory_inventory
    INTERFACE
    inventory
)

add_subdirectory(gpio_manager)
add_subdirectory(movement_manager)
add_subdirectory(stepper_motor_manager)