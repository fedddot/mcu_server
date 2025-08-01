add_library(ring_queue INTERFACE)
target_include_directories(ring_queue INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(ring_queue INTERFACE ipc_queue)

add_library(package_utils INTERFACE)
target_sources(package_utils INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/package_utils.cpp)
target_include_directories(package_utils INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(package_reader INTERFACE)
target_include_directories(package_reader INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    package_reader
    INTERFACE
    ipc_data_reader
    ipc_queue
)

add_library(package_writer INTERFACE)
target_include_directories(package_writer INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    package_writer
    INTERFACE
    ipc_data_writer
)

add_library(api_request_reader INTERFACE)
target_include_directories(api_request_reader INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    api_request_reader
    INTERFACE
    ipc_data_reader
    ipc_queue
    package_reader
)

add_library(api_response_writer INTERFACE)
target_include_directories(api_response_writer INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    api_response_writer
    INTERFACE
    ipc_data_writer
    package_writer
)

add_subdirectory(tests EXCLUDE_FROM_ALL)