add_library(thermostat_manager INTERFACE)
target_include_directories(thermostat_manager INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    thermostat_manager
    INTERFACE
    manager
    manager_instance
    thermostat_manager_controller
)

add_library(thermostat_manager_controller INTERFACE)
target_include_directories(thermostat_manager_controller INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(thermostat_manager_controller INTERFACE manager_instance)

add_subdirectory(tests EXCLUDE_FROM_ALL)