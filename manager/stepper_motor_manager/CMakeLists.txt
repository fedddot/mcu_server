add_library(stepper_motor INTERFACE)
target_include_directories(stepper_motor INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(stepper_motor INTERFACE stepper_motor_types)

add_library(stepper_motor_creator INTERFACE)
target_include_directories(stepper_motor_creator INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(stepper_motor_creator INTERFACE provider stepper_motor)

add_library(stepper_motor_delay_generator INTERFACE)
target_include_directories(stepper_motor_delay_generator INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(stepper_motor_delay_generator INTERFACE provider)


add_library(stepper_motor_manager INTERFACE)
target_include_directories(stepper_motor_manager INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    stepper_motor_manager
    INTERFACE
    manager
    provider
    providers
    stepper_motor
    stepper_motor_creator
    stepper_motor_delay_generator
    stepper_motor_request
    stepper_motor_response
    stepper_motor_types
)

add_subdirectory(data)
add_subdirectory(tests EXCLUDE_FROM_ALL)