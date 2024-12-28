add_library(http_ipc_connection INTERFACE)
target_sources(http_ipc_connection INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/http_ipc_connection.cpp)
target_include_directories(http_ipc_connection INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    http_ipc_connection
    INTERFACE
    cpprest
)

add_subdirectory(tests EXCLUDE_FROM_ALL)