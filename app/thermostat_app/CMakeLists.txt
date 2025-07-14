add_library(thermostat_host_builder INTERFACE)
target_include_directories(thermostat_host_builder INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    thermostat_host_builder
    INTERFACE
    host
    host_instance
    ipc_data_reader
    ipc_data_writer
    thermostat_manager
    thermostat_vendor_api_request
    thermostat_vendor_api_response
    raw_data_package_reader
    raw_data_package_writer
    api_request_reader_builder
    api_response_writer_builder
)

add_subdirectory(tests EXCLUDE_FROM_ALL)