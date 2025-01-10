add_library(ipc_server INTERFACE)
target_include_directories(ipc_server INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(ipc_config INTERFACE)
target_include_directories(ipc_config INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(http_ipc_server INTERFACE)
target_include_directories(http_ipc_server INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    http_ipc_server
    INTERFACE
    ipc_config
    ipc_server
    cpprest
)

add_library(ipc_server_factory INTERFACE)
target_include_directories(ipc_server_factory INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    ipc_server_factory
    INTERFACE
    ipc_config
    ipc_server
    test_ipc_server
    http_ipc_server
)

add_subdirectory(tests EXCLUDE_FROM_ALL)