add_library(host INTERFACE)
target_include_directories(host INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    host
    INTERFACE
    ipc_data_reader
    ipc_data_reader
    ipc_clonable
    
    manager
    manager_clonable
)

add_subdirectory(tests EXCLUDE_FROM_ALL)