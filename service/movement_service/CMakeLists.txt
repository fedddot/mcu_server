add_library(movement_api_request INTERFACE)
target_include_directories(movement_api_request INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(axes_controller_config_request INTERFACE)
target_include_directories(axes_controller_config_request INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(axes_controller_config_request INTERFACE movement_api_request)

add_library(linear_movement_request INTERFACE)
target_include_directories(linear_movement_request INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(linear_movement_request INTERFACE movement_api_request movement_manager_vector)

add_library(rotation_movement_request INTERFACE)
target_include_directories(rotation_movement_request INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(rotation_movement_request INTERFACE movement_api_request movement_manager_vector)


add_library(movement_service_api_response INTERFACE)
target_include_directories(movement_service_api_response INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(movement_service INTERFACE)
target_include_directories(movement_service INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    movement_service
    INTERFACE
    axes_controller_config_request
    linear_movement_request
    rotation_movement_request
    movement_manager
    movement_api_request
    movement_service_api_response
    service
    service_instance
)

add_subdirectory(movement_manager EXCLUDE_FROM_ALL)
add_subdirectory(tests EXCLUDE_FROM_ALL)