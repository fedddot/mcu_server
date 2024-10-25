add_library(cnc_server INTERFACE)
target_include_directories(cnc_server INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    cnc_server
    INTERFACE
    server
    data_lib
    ipc_connection
    request
    response
    resources_vendor
    server_exception
    server_types
    stepper_motor_manager
    movement_manager
    vendor
)

add_subdirectory(tests EXCLUDE_FROM_ALL)