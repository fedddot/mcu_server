add_executable(test_stepper_motor_server ${CMAKE_CURRENT_LIST_DIR}/src/test_stepper_motor_server.cpp)
target_link_libraries(
    test_stepper_motor_server
    PRIVATE
    host
    http_ipc_server
    manager_factory
    stepper_motor_request
    stepper_motor_response
    
    jsoncpp_static
    cpprest
)