add_library(ipc_data_reader INTERFACE)
target_include_directories(ipc_data_reader INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(ipc_data_writer INTERFACE)
target_include_directories(ipc_data_writer INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(data_buffer INTERFACE)
target_include_directories(data_buffer INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(ring_data_buffer INTERFACE)
target_include_directories(ring_data_buffer INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(ring_data_buffer INTERFACE data_buffer)

add_subdirectory(package_reader)
add_subdirectory(tests EXCLUDE_FROM_ALL)