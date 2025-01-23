add_library(ipc_server INTERFACE)
target_include_directories(ipc_server INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(http_ipc_server INTERFACE)
target_include_directories(http_ipc_server INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    http_ipc_server
    INTERFACE
    ipc_server
    ipc_option

    cpprest
    boost_system
	crypto
    pthread
)

add_library(buffered_ipc_server INTERFACE)
target_include_directories(buffered_ipc_server INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    buffered_ipc_server
    INTERFACE
    ipc_option
    ipc_server
)

add_subdirectory(tests EXCLUDE_FROM_ALL)