add_library(thermostat_vendor INTERFACE)
target_include_directories(thermostat_vendor INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(
    thermostat_vendor
    INTERFACE
    vendor
    vendor_instance
    thermostat_api_request
    thermostat_api_response
    thermostat_manager
)

add_library(thermostat_api_request INTERFACE)
target_include_directories(thermostat_api_request INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(thermostat_api_response INTERFACE)
target_include_directories(thermostat_api_response INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_subdirectory(tests EXCLUDE_FROM_ALL)