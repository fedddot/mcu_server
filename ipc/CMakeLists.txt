add_library(ipc_connection INTERFACE)
target_include_directories(ipc_connection INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(http_ipc_connection INTERFACE)
target_include_directories(http_ipc_connection INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    http_ipc_connection
    INTERFACE
    ipc_connection
    cpprest
)

add_subdirectory(tests EXCLUDE_FROM_ALL)