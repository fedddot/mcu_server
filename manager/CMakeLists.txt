add_library(provider INTERFACE)
target_include_directories(provider INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(manager_config INTERFACE)
target_include_directories(manager_config INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(manager INTERFACE)
target_include_directories(manager INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(manager_factory INTERFACE)
target_include_directories(manager_factory INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    manager_factory
    INTERFACE
    manager
    manager_config
    stepper_motor_manager
    stepper_motor_manager_config
)

add_subdirectory(stepper_motor_manager)
add_subdirectory(tests EXCLUDE_FROM_ALL)