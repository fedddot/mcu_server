add_library(host INTERFACE)
target_include_directories(host INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    host
    INTERFACE
    ipc_data_reader
    clonable_ipc_data_reader

    ipc_data_writer
    clonable_ipc_data_writer
    
    manager
    clonable_manager
)

add_subdirectory(tests EXCLUDE_FROM_ALL)