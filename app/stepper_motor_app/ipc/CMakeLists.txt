add_library(stepper_motor_http_ipc_server INTERFACE)
target_include_directories(stepper_motor_http_ipc_server INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    stepper_motor_http_ipc_server
    INTERFACE
    ipc_connection
    http_ipc_connection
    stepper_motor_request
    stepper_motor_request_data
    stepper_motor_response
    stepper_motor_types

    jsoncpp_static
)