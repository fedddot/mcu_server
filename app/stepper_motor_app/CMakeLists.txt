add_executable(test_stepper_motor_server ${CMAKE_CURRENT_LIST_DIR}/src/test_stepper_motor_server.cpp)
target_link_libraries(
    test_stepper_motor_server
    PRIVATE
    host
    http_ipc_server
    ipc_server_factory
    manager_factory
    stepper_motor_manager_config
    stepper_motor_request
    stepper_motor_response
    stepper_motor_types
    test_stepper_motor_creator
    test_stepper_motor_delay_generator
    
    jsoncpp_static
    cpprest
)