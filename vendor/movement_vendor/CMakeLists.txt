add_library(movement_vendor_manager_id INTERFACE)
target_include_directories(movement_vendor_manager_id INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(movement_vendor_api_request INTERFACE)
target_include_directories(movement_vendor_api_request INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    movement_vendor_api_request
    INTERFACE
    movement_manager_request
    movement_vendor_manager_id
    vendor_api_request
    vendor_instance
)

add_library(movement_vendor_api_response INTERFACE)
target_include_directories(movement_vendor_api_response INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    movement_vendor_api_response
    INTERFACE
    movement_manager_response
    vendor_api_response
)

add_library(movement_vendor INTERFACE)
target_include_directories(movement_vendor INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    movement_vendor
    INTERFACE
    manager
    movement_manager_request
    movement_manager_response
    movement_vendor_api_response
    movement_vendor_manager_id
    vendor
    vendor_api_response
    vendor_instance
)

add_subdirectory(tests EXCLUDE_FROM_ALL)