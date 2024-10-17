add_library(inventory_manager INTERFACE)
target_include_directories(inventory_manager INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    inventory_manager
    INTERFACE
    request
    resource
    response
    data_lib
)

add_library(requests_manager INTERFACE)
target_include_directories(requests_manager INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    requests_manager
    INTERFACE
    data
    request
    resource
    response
)

add_subdirectory(gpio_manager)
add_subdirectory(stepper_motor_manager)

add_subdirectory(tests EXCLUDE_FROM_ALL)