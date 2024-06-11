add_library(server INTERFACE)
target_include_directories(server INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    server
    INTERFACE
    connection
    engine
    functional_listener
)

add_subdirectory(tests EXCLUDE_FROM_ALL)