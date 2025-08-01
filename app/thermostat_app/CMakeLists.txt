add_library(thermostat_app INTERFACE)
target_include_directories(thermostat_app INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    thermostat_app
    INTERFACE
    api_request_reader
    api_response_writer
    host
    ipc_queue
    thermostat_api_request
    thermostat_api_response
    thermostat_controller
    thermostat_service
)

add_subdirectory(proto)
add_subdirectory(tests EXCLUDE_FROM_ALL)