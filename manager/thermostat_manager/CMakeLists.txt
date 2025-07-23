add_library(thermostat_manager INTERFACE)
target_include_directories(thermostat_manager INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    thermostat_manager
    INTERFACE
    manager
    manager_instance_reference
    relay_controller
    temperature_sensor_controller
    timer_scheduler
)

add_library(relay_controller INTERFACE)
target_include_directories(relay_controller INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(temperature_sensor_controller INTERFACE)
target_include_directories(temperature_sensor_controller INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(timer_scheduler INTERFACE)
target_include_directories(timer_scheduler INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(timer_scheduler INTERFACE manager_instance)

add_subdirectory(tests EXCLUDE_FROM_ALL)