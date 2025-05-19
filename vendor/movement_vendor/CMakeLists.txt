add_library(movement_vendor_api_request INTERFACE)
target_include_directories(movement_vendor_api_request INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(movement_vendor_api_response INTERFACE)
target_include_directories(movement_vendor_api_response INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(movement_vendor INTERFACE)
target_include_directories(movement_vendor INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    movement_vendor
    INTERFACE
    axes_controller_config_request
    movement_manager
    movement_vendor_api_request
    movement_vendor_api_response
    vendor
    vendor_instance
)

add_subdirectory(tests EXCLUDE_FROM_ALL)