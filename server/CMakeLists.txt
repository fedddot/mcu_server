add_library(server INTERFACE)
target_include_directories(server INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    server
    INTERFACE
    engine
    connection
    server_task_engine
)

add_subdirectory(task_engine)
add_subdirectory(tests EXCLUDE_FROM_ALL)