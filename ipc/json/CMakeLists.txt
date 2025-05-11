add_library(json_ipc_data_reader INTERFACE)
target_include_directories(json_ipc_data_reader INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    json_ipc_data_reader
    INTERFACE
    ipc_instance
    ipc_data
    ipc_data_reader
    jsoncpp_static
)

add_library(json_ipc_data_writer INTERFACE)
target_include_directories(json_ipc_data_writer INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    json_ipc_data_writer
    INTERFACE
    ipc_instance
    ipc_data
    ipc_data_writer
    jsoncpp_static
)

add_subdirectory(tests EXCLUDE_FROM_ALL)