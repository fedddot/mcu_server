add_library(thermostat_manager INTERFACE)
target_include_directories(thermostat_manager INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    thermostat_manager
    INTERFACE
    manager
)

add_library(relay_controller INTERFACE)
target_include_directories(relay_controller INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(temperature_sensor_controller INTERFACE)
target_include_directories(temperature_sensor_controller INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_subdirectory(tests EXCLUDE_FROM_ALL)