add_library(gpio INTERFACE)
target_include_directories(gpio INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(gpi INTERFACE)
target_include_directories(gpi INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(gpi INTERFACE gpio)

add_library(gpo INTERFACE)
target_include_directories(gpo INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(gpo INTERFACE gpio)

add_library(gpio_request INTERFACE)
target_include_directories(gpio_request INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(gpio_request INTERFACE data)

add_library(gpio_response INTERFACE)
target_include_directories(gpio_response INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
# target_link_libraries(gpio_response INTERFACE gpio)

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
    gpo
    gpio_provider
    gpio_response
    gpio_request
    manager
)

add_subdirectory(tests EXCLUDE_FROM_ALL)