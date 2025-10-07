add_library(motor_drive_service_types INTERFACE)
target_include_directories(motor_drive_service_types INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(motor_drive_service_api_request INTERFACE)
target_include_directories(motor_drive_service_api_request INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(motor_drive_service_api_request INTERFACE motor_drive_service_types)

add_library(motor_drive_service_api_response INTERFACE)
target_include_directories(motor_drive_service_api_response INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(motor_drive_service_api_response INTERFACE motor_drive_service_types)

add_library(motor_drive_service INTERFACE)
target_include_directories(motor_drive_service INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    motor_drive_service
    INTERFACE
    service
    motor_drive_service_api_request
    motor_drive_service_api_response
    pwm_controller
    digital_output_controller
)

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()