add_library(movement_json_api_request_parser INTERFACE)
target_include_directories(movement_json_api_request_parser INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    movement_json_api_request_parser
    INTERFACE
    axes_controller_config_request
    linear_movement_request
    ipc_instance
    movement_manager_data
    movement_manager_vector
    movement_service_api_request
    rotation_movement_request
    jsoncpp_static
)

add_library(movement_json_api_response_serializer INTERFACE)
target_include_directories(movement_json_api_response_serializer INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    movement_json_api_response_serializer
    INTERFACE
    movement_service_api_response
    jsoncpp_static
)

add_library(movement_host_builder INTERFACE)
target_include_directories(movement_host_builder INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    movement_host_builder
    INTERFACE
    host
    host_instance
    ipc_data_reader
    ipc_data_writer
    movement_manager
    movement_service_api_request
    movement_service_api_response
    raw_data_package_reader
    raw_data_package_writer
    api_request_reader_builder
    api_response_writer_builder
)

add_library(file_raw_data_reader INTERFACE)
target_include_directories(file_raw_data_reader INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    file_raw_data_reader
    INTERFACE
    ipc_data
    ipc_data_reader
    ipc_instance
    jsoncpp_static
)

add_executable(movement_test_cli_app ${CMAKE_CURRENT_LIST_DIR}/src/movement_test_cli_app.cpp)
target_link_libraries(
    movement_test_cli_app
    PRIVATE
    axes_controller
    manager_instance
    movement_host_builder
    movement_json_api_request_parser
    movement_json_api_response_serializer
    movement_manager_data
    movement_service_api_response
    file_raw_data_reader
)

add_subdirectory(tests EXCLUDE_FROM_ALL)