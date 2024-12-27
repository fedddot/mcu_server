add_library(cnc_server INTERFACE)
target_include_directories(cnc_server INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    cnc_server
    INTERFACE
    circular_movement
    clonable_manager_wrapper
    custom_server
    data_lib
    data_reader
    data_writer
    gpio
    gpio_manager
    in_memory_inventory
    linear_movement
    movement
    movement_manager
    request
    response
    server
    server_exception
    server_types
    stepper_motor
    stepper_motor_manager
    vector
    vendor
)

add_library(test_http_cnc_server INTERFACE)
target_include_directories(test_http_cnc_server INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    test_http_cnc_server
    INTERFACE
    cnc_server
    data_lib
    gpio
    http_ipc_connection
    server
    test_gpi
    test_gpo
)

add_executable(test_http_cnc_server_app ${CMAKE_CURRENT_LIST_DIR}/src/test_http_cnc_server.cpp)
target_link_libraries(test_http_cnc_server_app PRIVATE test_http_cnc_server)

add_subdirectory(tests EXCLUDE_FROM_ALL)