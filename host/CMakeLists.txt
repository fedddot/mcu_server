add_library(host INTERFACE)
target_include_directories(host INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    host
    INTERFACE
    ipc_connection
    manager
    providers
)

add_subdirectory(tests EXCLUDE_FROM_ALL)