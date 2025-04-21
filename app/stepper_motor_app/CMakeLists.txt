add_library(stepper_ipc_data_reader INTERFACE)
target_include_directories(stepper_ipc_data_reader INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    stepper_ipc_data_reader
    INTERFACE
    clonable_ipc_data_reader
    ipc_data_reader
    json_ipc_data_reader
    stepper_motor
    stepper_motor_request
    
    jsoncpp_static
)

add_library(stepper_ipc_data_writer INTERFACE)
target_include_directories(stepper_ipc_data_writer INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    stepper_ipc_data_writer
    INTERFACE
    clonable_ipc_data_writer
    ipc_data_writer
    json_ipc_data_writer
    stepper_motor_response
    
    jsoncpp_static
)

# add_executable(test_stepper_motor_server ${CMAKE_CURRENT_LIST_DIR}/src/test_stepper_motor_server.cpp)
# target_link_libraries(
#     test_stepper_motor_server
#     PRIVATE
#     host
#     http_ipc_server
#     stepper_motor_request
#     stepper_motor_response
#     test_stepper_motor
    
#     steppercpp_static
#     cpprest
# )

add_subdirectory(tests EXCLUDE_FROM_ALL)