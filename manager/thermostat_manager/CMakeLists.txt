add_library(thermostat_manager INTERFACE)
target_include_directories(thermostat_manager INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    thermostat_manager
    INTERFACE
    manager
)

add_subdirectory(tests EXCLUDE_FROM_ALL)