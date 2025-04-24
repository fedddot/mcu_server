add_library(movement_manager INTERFACE)
target_include_directories(movement_manager INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    movement_manager
    INTERFACE
    manager
    clonable_manager
    movement_manager_request
    movement_manager_response
)

add_subdirectory(data)
add_subdirectory(tests EXCLUDE_FROM_ALL)