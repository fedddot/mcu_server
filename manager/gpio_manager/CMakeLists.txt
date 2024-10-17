add_library(gpio INTERFACE)
target_include_directories(gpio INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(gpi INTERFACE)
target_include_directories(gpi INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(gpi INTERFACE gpio)

add_library(gpo INTERFACE)
target_include_directories(gpo INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(gpo INTERFACE gpio)

add_library(gpio_manager INTERFACE)
target_include_directories(gpio_manager INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    gpio_manager
    INTERFACE
    data_lib
    gpio
    inventory
    inventory_manager
    request
    resource
    response
)

add_subdirectory(tests EXCLUDE_FROM_ALL)