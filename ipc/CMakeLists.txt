add_library(ipc_server INTERFACE)
target_include_directories(ipc_server INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(http_ipc_server INTERFACE)
target_include_directories(http_ipc_server INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    http_ipc_server
    INTERFACE
    ipc_server
    cpprest
)

add_subdirectory(tests EXCLUDE_FROM_ALL)