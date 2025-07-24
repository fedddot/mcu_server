add_library(host_instance INTERFACE)
target_include_directories(host_instance INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(host INTERFACE)
target_include_directories(host INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    host
    INTERFACE
    host_instance
    service
    ipc_data_reader
    ipc_data_writer
)

add_subdirectory(tests EXCLUDE_FROM_ALL)