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
    thermostat_api_request
    thermostat_api_response
    thermostat_manager
    thermostat_manager_controller
    thermostat_vendor
)

add_subdirectory(proto)
add_subdirectory(tests EXCLUDE_FROM_ALL)