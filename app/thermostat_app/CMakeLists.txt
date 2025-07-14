add_library(thermostat_host_builder INTERFACE)
target_include_directories(thermostat_host_builder INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    thermostat_host_builder
    INTERFACE
    api_request_reader_builder
    api_response_writer_builder
    host
    ipc_data
    ipc_data_reader
    ipc_data_writer
    ipc_instance
    manager_instance
    relay_controller
    temperature_sensor_controller
    thermostat_api_request
    thermostat_api_response
    thermostat_manager
    thermostat_vendor
    timer_scheduler
)

add_subdirectory(tests EXCLUDE_FROM_ALL)