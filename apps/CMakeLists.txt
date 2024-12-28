add_library(gpio_app_types INTERFACE)
target_include_directories(gpio_app_types INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(gpio_app_types INTERFACE data_lib)

add_library(gpio_ipc_factory INTERFACE)
target_include_directories(gpio_ipc_factory INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    gpio_ipc_factory
    INTERFACE
    data_lib
    gpio_app_types
    gpio_request
    ipc_connection
    http_ipc_connection
)

add_library(gpio_host INTERFACE)
target_include_directories(gpio_host INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    gpio_host
    INTERFACE
    data_lib
    host
    gpio_manager
    gpio_create_request
    gpio_ipc_factory
    gpio_provider
    gpio_regular_response
    gpio_request
    gpio_response
    gpio_state_response
    gpio_write_request
)

add_subdirectory(tests EXCLUDE_FROM_ALL)