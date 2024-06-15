add_library(gpio INTERFACE)
target_include_directories(gpio INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(gpi INTERFACE)
target_include_directories(gpi INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(gpi INTERFACE gpio)

add_library(gpo INTERFACE)
target_include_directories(gpo INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(gpo INTERFACE gpio)

add_library(inventory INTERFACE)
target_include_directories(inventory INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(server_factory INTERFACE)
target_include_directories(server_factory INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(server_factory INTERFACE creator parser data_lib task inventory gpio task_factory functional_creator functional_task)

add_library(server INTERFACE)
target_include_directories(server INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    server
    INTERFACE
    connection
    engine
    inventory
    server_factory
)

add_subdirectory(tests EXCLUDE_FROM_ALL)