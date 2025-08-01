add_library(thermostat_app INTERFACE)
target_include_directories(thermostat_app INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    thermostat_app
    INTERFACE
    host
    thermostat_api_request
    thermostat_api_response
    thermostat_controller
    thermostat_service
    api_request_reader
)

add_subdirectory(proto)
add_subdirectory(tests EXCLUDE_FROM_ALL)