# add_library(response_json_transformer INTERFACE)
# target_include_directories(response_json_transformer INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
# target_link_libraries(
#     response_json_transformer
#     INTERFACE
#     movement_manager_data
#     movement_manager_request
#     movement_manager_response
#     movement_manager_vector
#     ipc_instance
#     jsoncpp_static
# )

# add_library(request_json_transformer INTERFACE)
# target_include_directories(request_json_transformer INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
# target_link_libraries(
#     request_json_transformer
#     INTERFACE
#     movement_manager_data
#     movement_manager_request
#     movement_manager_response
#     movement_manager_vector
#     ipc_instance
#     jsoncpp_static
# )

# add_library(movement_ipc_data_reader INTERFACE)
# target_include_directories(movement_ipc_data_reader INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
# target_link_libraries(
#     movement_ipc_data_reader
#     INTERFACE
#     ipc_instance
#     ipc_data
#     ipc_data_reader
#     ipc_instance
#     json_ipc_data_reader
#     request_json_transformer
#     movement_manager_request
# )

# add_library(movement_ipc_data_writer INTERFACE)
# target_include_directories(movement_ipc_data_writer INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
# target_link_libraries(
#     movement_ipc_data_writer
#     INTERFACE
#     ipc_instance
#     ipc_data_writer
#     json_ipc_data_writer
#     response_json_transformer
#     movement_manager_response
# )

add_library(movement_host_builder INTERFACE)
target_include_directories(movement_host_builder INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    movement_host_builder
    INTERFACE
    host
    host_instance
    ipc_data_reader
    ipc_data_writer
    movement_vendor_api_request
    movement_vendor_api_response
    raw_data_package_reader
    raw_data_package_writer
    api_request_reader_builder
    api_response_writer_builder
)

add_subdirectory(tests EXCLUDE_FROM_ALL)