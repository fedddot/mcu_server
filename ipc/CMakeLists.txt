add_library(ipc_data_reader INTERFACE)
target_include_directories(ipc_data_reader INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(ipc_data_writer INTERFACE)
target_include_directories(ipc_data_writer INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(ipc_queue INTERFACE)
target_include_directories(ipc_queue INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_subdirectory(api_request_reader)
add_subdirectory(tools)
add_subdirectory(package_reader)
add_subdirectory(tests EXCLUDE_FROM_ALL)