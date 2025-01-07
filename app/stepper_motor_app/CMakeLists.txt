add_executable(test_stepper_motor_server ${CMAKE_CURRENT_LIST_DIR}/src/test_stepper_motor_server.cpp)
target_link_libraries(
    test_stepper_motor_server
    PRIVATE
    host
    ipc_connection
    manager
    providers
    stepper_motor_http_ipc_server
    stepper_motor_manager
    stepper_motor_request
    stepper_motor_request_data
    stepper_motor_response
    stepper_motor_types
    jsoncpp_static
)

add_subdirectory(ipc)
add_subdirectory(manager)