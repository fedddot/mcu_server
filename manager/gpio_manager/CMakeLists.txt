add_library(gpio INTERFACE)
target_include_directories(gpio INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(gpi INTERFACE)
target_include_directories(gpi INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(gpi INTERFACE gpio)

add_library(gpo INTERFACE)
target_include_directories(gpo INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(gpo INTERFACE gpio)

add_library(gpio_provider INTERFACE)
target_include_directories(gpio_provider INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(gpio_provider INTERFACE provider)

add_library(gpio_manager INTERFACE)
target_include_directories(gpio_manager INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    gpio_manager
    INTERFACE
    gpi
    gpio
    gpio_create_request
    gpio_provider
    gpio_regular_response
    gpio_request
    gpio_response
    gpio_response_wrapper
    gpio_state_response
    gpio_write_request
    gpo
    manager
    provider
)

add_subdirectory(data)
add_subdirectory(tests EXCLUDE_FROM_ALL)