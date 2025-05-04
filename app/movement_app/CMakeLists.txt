add_library(movement_ipc_data_infra INTERFACE)
target_include_directories(movement_ipc_data_infra INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    movement_ipc_data_infra
    INTERFACE
    movement_manager_data
    movement_manager_request
    movement_manager_response
    movement_manager_vector
    jsoncpp_static
)

add_library(movement_ipc_data_reader INTERFACE)
target_include_directories(movement_ipc_data_reader INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    movement_ipc_data_reader
    INTERFACE
    clonable_ipc_data_reader
    ipc_data_reader
    json_ipc_data_reader
    movement_ipc_data_infra
    movement_manager_request
)

add_library(movement_ipc_data_writer INTERFACE)
target_include_directories(movement_ipc_data_writer INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    movement_ipc_data_writer
    INTERFACE
    clonable_ipc_data_writer
    ipc_data_writer
    json_ipc_data_writer
    movement_ipc_data_infra
    movement_manager_response
)

add_library(movement_host INTERFACE)
target_include_directories(movement_host INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    movement_host
    INTERFACE
    host
    movement_ipc_data_infra
    movement_ipc_data_reader
    movement_ipc_data_writer
    movement_manager
    movement_manager_data
    movement_manager_request
    movement_manager_response
)

add_subdirectory(tests EXCLUDE_FROM_ALL)