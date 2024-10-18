add_library(movement_manager INTERFACE)
target_include_directories(movement_manager INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    movement_manager
    INTERFACE
    data_lib
    inventory
    stepper_motor
    manager
    server_exception
    server_types
    movement
)

add_subdirectory(tests EXCLUDE_FROM_ALL)