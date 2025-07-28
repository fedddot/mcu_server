add_library(data_buffer INTERFACE)
target_include_directories(data_buffer INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(ring_data_buffer INTERFACE)
target_include_directories(ring_data_buffer INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(ring_data_buffer INTERFACE data_buffer)

add_library(api_request_reader INTERFACE)
target_include_directories(api_request_reader INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(api_request_reader INTERFACE data_buffer)


add_subdirectory(tests EXCLUDE_FROM_ALL)