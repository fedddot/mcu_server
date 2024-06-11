add_library(creator INTERFACE)
target_include_directories(creator INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(task INTERFACE)
target_include_directories(task INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(engine INTERFACE)
target_include_directories(engine INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    engine
    INTERFACE
    creator
    parser
    task
)

add_subdirectory(data)
add_subdirectory(tests EXCLUDE_FROM_ALL)