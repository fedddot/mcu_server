

add_library(server_tasks INTERFACE)
target_include_directories(server_tasks INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(server_factory INTERFACE)
target_include_directories(server_factory INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(server_factory INTERFACE creator parser data_lib task inventory gpio factory functional_creator functional_task server_tasks inventory_task)

add_library(server INTERFACE)
target_include_directories(server INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    server
    INTERFACE
    connection
    engine
    inventory
    server_factory
    server_tasks
)

add_subdirectory(task_engine)
add_subdirectory(tests EXCLUDE_FROM_ALL)