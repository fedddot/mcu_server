add_library(thermostat_service INTERFACE)
target_include_directories(thermostat_service INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(
    thermostat_service
    INTERFACE
    thermostat_api_request
    thermostat_api_response
    service
    thermostat_controller
)

add_library(thermostat_api_request INTERFACE)
target_include_directories(thermostat_api_request INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(thermostat_api_response INTERFACE)
target_include_directories(thermostat_api_response INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(thermostat_controller INTERFACE)
target_include_directories(thermostat_controller INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_subdirectory(tests EXCLUDE_FROM_ALL)