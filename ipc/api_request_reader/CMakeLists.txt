add_library(api_request_reader INTERFACE)
target_include_directories(api_request_reader INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    api_request_reader
    INTERFACE
    ipc_data_reader
    ipc_queue
    package_reader
)

add_subdirectory(tests EXCLUDE_FROM_ALL)