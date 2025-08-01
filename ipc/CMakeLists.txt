add_library(ipc_data_reader INTERFACE)
target_include_directories(ipc_data_reader INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(ipc_data_writer INTERFACE)
target_include_directories(ipc_data_writer INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(ipc_queue INTERFACE)
target_include_directories(ipc_queue INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_subdirectory(tools)
add_subdirectory(tests EXCLUDE_FROM_ALL)