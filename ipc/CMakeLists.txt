add_library(ipc_instance INTERFACE)
target_include_directories(ipc_instance INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(ipc_data INTERFACE)
target_include_directories(ipc_data INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(ipc_data_reader INTERFACE)
target_include_directories(ipc_data_reader INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(api_request_reader_builder INTERFACE)
target_include_directories(api_request_reader_builder INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(api_request_reader_builder INTERFACE ipc_data_reader)

add_library(ipc_data_writer INTERFACE)
target_include_directories(ipc_data_writer INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_subdirectory(raw_data_package)
add_subdirectory(json)
add_subdirectory(tests EXCLUDE_FROM_ALL)