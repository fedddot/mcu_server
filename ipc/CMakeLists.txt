add_library(ipc_connection INTERFACE)
target_include_directories(ipc_connection INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(ipc_connection INTERFACE request response)

add_library(buffered_ipc_connection INTERFACE)
target_include_directories(buffered_ipc_connection INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(buffered_ipc_connection INTERFACE ipc_connection request response)

add_subdirectory(tests)